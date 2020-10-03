#include <iostream>

#include "csv.h"

int main () {
  csv::Collection life_expectancy("WPP2015_MORT_F07_1_LIFE_EXPECTANCY_0_BOTH_SEXES.csv");
  for (auto e : life_expectancy[17]) {
    std::cout << e << "\t";
  }
  std::cout << std::endl;
}