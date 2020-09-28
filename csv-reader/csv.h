#include <vector>
#include <string>
#include <iostream>

#ifndef AIDEN_CSV_H
#define AIDEN_CSV_H

namespace csv {
  class Collection {
    using Row = std::vector<std::string>;
    using Row_index = decltype(Row::size);
    Row headers;
    std::vector<Row> data;

    bool read_file (std::string filename); // Overwrites data
    bool read_file (std::istream stream);
    Collection (std::string filename); // Should call Collection(std::istream)
    Collection (std::istream stream);
    Collection (decltype(headers) h, decltype(data) d);

    Row_index add_row (Row, Row_index position);
    Row&& remove_row (Row_index position);

    Row_index push_row (Row);
    Row&& pop_row ();

    Row_index unshift_row (Row);
    Row&& shift_row ();

    Row row (Row_index i) const;
    Row& row (Row_index i);

    Row operator[] (Row_index i) const noexcept;
    Row& operator[] (Row_index i) noexcept;
  };
} // namespace csv

#endif // AIDEN_CSV_H