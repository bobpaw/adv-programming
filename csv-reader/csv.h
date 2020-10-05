#include <vector>
#include <string>
#include <iostream>
#include <functional>

// Used by implementation but not by declaration
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#ifndef AIDEN_CSV_H
#define AIDEN_CSV_H

namespace csv {
  struct Collection {
    using Row = std::vector<std::string>;
    Row headers;
    std::vector<Row> data;
    using size_type = decltype(data)::size_type;

    // Destructive
    bool read_file (std::string filename, size_type header_line = -1) { // FIXME: Find another way to specify possibly no headers
      std::ifstream ifs;
      ifs.open(filename);
      bool ret = false;
      if (ifs) {
        ret = read_file(ifs, header_line);
      }
      ifs.close();
      return ret;
    }
    bool read_file (std::istream& stream, size_type header_line = -1) {
      if (stream.eof()) {
        return true; // i.e. empty object
      } else if (stream.fail()) {
        throw std::runtime_error("Failure opening the file or something.");
      } else {
        data.clear();
        Row next_row;
        std::string next_value;
        bool quoted = false;
        for (char ch = stream.get(); stream; stream.get(ch)) {
          if (quoted && ch != '"') {
            next_value += ch;
          } else {
              switch (ch) {
                  case '"':
                      if (stream.peek() == '"') {
                          next_value += ch;
                          stream.get();
                      } else {
                          quoted = !quoted;
                      }
                      break;
                  case ',':
                      next_row.push_back(next_value);
                      next_value.clear();
                      break;
                  case '\r': break;
                  case '\n':
                      if (next_row.size() == 0) continue;
                      next_row.push_back(next_value);
                      next_value.clear();
                      data.push_back(next_row);
                      next_row.clear();
                      break;
                  default:
                      next_value += ch;
              }
          }
        }
        if (header_line != -1 && header_line < data.size()) {
          headers = data[header_line - 1];
          data.erase(data.begin(), data.begin() + header_line); // Discard everything up to the headers
        }
        return stream.eof();
      }
    }

    Collection (std::string filename, size_type header_line = -1) {
        if (!read_file(filename, header_line)) throw std::runtime_error("Failed to construct object from filename.");
    }
    Collection (std::istream& stream, size_type header_line = -1): headers(), data() {
      if (!read_file(stream, header_line)) throw std::runtime_error("Failed to construct object from stream.");
    }
    Collection (decltype(headers) h, decltype(data) d): headers(h), data(d) {};

    size_type size () const {
        return data.size();
    }

    size_type add_row (Row v, size_type position) {
      auto i = data.insert(data.begin() + position, v);
      return std::distance(data.begin(), i);
    };
    void remove_row (size_type position) {
      data.erase(data.begin() + position);
    };

    /*
    // These should all call add_row or remove_row
    size_type push_row (Row);
    void pop_row ();
    size_type unshift_row (Row);
    void shift_row ();
    */

    const Row& row (size_type i) const { return data.at(i); }
    Row& row (size_type i) { return data.at(i); }

    const Row& operator[] (size_type i) const noexcept { return data[i]; }
    Row& operator[] (size_type i) noexcept { return data[i]; };

    // Obviously a column but they're the same thing
    Row operator[] (std::string heading) const {
        if (heading == "" || headers.size() == 0 || std::find(headers.begin(), headers.end(), heading) == headers.end()) return Row();
        Row ret;
        ret.reserve(size());
        Row::size_type col = std::distance(headers.begin(), std::find(headers.begin(), headers.end(), heading));
        for (Row e : data) ret.push_back(e[col]);
        return ret;
    }

    Row::size_type col (const std::string& heading) const {
        auto it = std::find(headers.begin(), headers.end(), heading);
        return it == headers.end() ? -1 : std::distance(headers.begin(), it);
    }

    // Calls std::sort using comp
    void sort (std::function<bool(const Row&, const Row&)> comp) {
        std::sort(data.begin(), data.end(), comp);
    }
  };
} // namespace csv

#endif // AIDEN_CSV_H