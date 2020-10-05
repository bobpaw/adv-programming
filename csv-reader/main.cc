#include <iostream>
#include <algorithm>

#include "csv.h"

int main () {
  csv::Collection life_expectancy("life_expectancy_asc.csv", 17);
  auto col = life_expectancy.col("2000-2005");
  life_expectancy.sort([col](const auto& a, const auto& b) {
	  return std::stof(a[col]) > std::stof(b[col]);
	  });
  std::cout << "Rank\tLE\tCountry/Region" << std::endl;
  for (int i = 0; i < life_expectancy.size(); ++i) {
	  std::cout << i + 1 << '\t' << life_expectancy[i][col] << '\t' << life_expectancy[i][2] << std::endl;
  }
}