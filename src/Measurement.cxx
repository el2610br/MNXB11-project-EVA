#include "Measurement.h"
#include <iostream>


Measurement::Measurement(date::year_month_day date, std::string time, double temperature)
    : date{date}, time{time}, temperature{temperature} {

  //Validation checks

  //Check that the time is valid
  int h = std::stoi(time.substr(0,2)); int m = std::stoi(time.substr(3,2)); int s = std::stoi(time.substr(6,2));

  if ((h < 0 || h > 24) || (m < 0 || m > 60) || (s < 0 || s > 60)) {
    throw std::invalid_argument("Invalid time found!");
  }

  //Check that temperature isn't an infinite number
  if (std::isinf(temperature)) {
    throw std::invalid_argument("Infinite measurement found!");
  }

  //Check that temperature isn't NaN (not a number)
  if (std::isnan(temperature)) {
    throw std::invalid_argument("Infinite measurement found!");
  }

}