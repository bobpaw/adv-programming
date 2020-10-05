#include <iostream>

#include "csv.h"

int main () {
  csv::Collection life_expectancy("life_expectancy_asc.csv");
  for (auto e : life_expectancy[17]) {
      std::cout << e << "\t";
  }
  std::cout << std::endl;
}