#include <iostream>
#include <algorithm>

#include "csv.h"

int main () {
  csv::Collection life_expectancy("life_expectancy_asc.csv", 17);

  // Remove WORLD and income/development groups
  life_expectancy.remove_rows(0, 11);
  
  // Remove regions
  for (auto region : {"AFRICA", "ASIA", "EUROPE", "LATIN AMERICA AND THE CARIBBEAN", "NORTHERN AMERICA", "OCEANIA"})
  life_expectancy.remove_row(life_expectancy.row([region](const auto& a) { return a[2] == region; }));

  auto col = life_expectancy.col("2000-2005");
  life_expectancy.sort([col](const auto& a, const auto& b) {
	  return std::stof(a[col]) > std::stof(b[col]);
	  });

  std::cout << "Overall ranking" << std::endl;
  std::cout << "Rank\tLE\tCountry/Region" << std::endl;
  for (int i = 0; i < life_expectancy.size(); ++i) {
	  std::cout << i + 1 << '\t' << life_expectancy[i][col] << '\t' << life_expectancy[i][2] << std::endl;
  }
}