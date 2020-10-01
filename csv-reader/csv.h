#include <vector>
#include <string>
#include <iostream>

// Used by implementation but not by declaration
#include <iterator>
#include <fstream>
#include <stdexcept>

#ifndef AIDEN_CSV_H
#define AIDEN_CSV_H

namespace csv {
  class Collection {
    using Row = std::vector<std::string>;
    Row headers;
    std::vector<Row> data;
    using size_type = decltype(data)::size_type;

    public:

    bool read_file (std::string filename, int header_line); // Overwrites data
    bool read_file (std::istream stream, int header_line);

    Collection (std::string filename): Collection(std::ifstream(filename)) {}
    Collection (std::istream stream): headers(), data() {
      if (stream.eof()) {
        return; // i.e. empty object
      } else if (stream.fail()) {
        throw new std::runtime_error("Failure opening the file or something.");
      } else {
        char ch;
        Row next_row;
        std::string next_value;
        bool quoted = false;
        while (stream >> ch) {
          if (quoted && ch != '"') {
            next_value += ch;
            continue;
          }
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
              if (next_row.size() && next_value.size() == 0) continue;
              next_row.push_back(next_value);
              next_value.clear();
              data.push_back(next_row);
              next_row.clear();
              break;
            default:
              next_value += ch;
          }
        }
        if (stream.eof()); // Success
        else {
          std::cerr << "An error occurred." << std::endl;
          throw std::runtime_error("Error reading file");
        }
      }
    }
    Collection (decltype(headers) h, decltype(data) d): headers(h), data(d) {};

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
  };
} // namespace csv

#endif // AIDEN_CSV_H