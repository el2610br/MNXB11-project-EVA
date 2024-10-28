#ifndef MEASUREMENT_H
#define MEASUREMENT_H
#include <cmath>
#include <stdexcept>
#include "argh.h"
#include "csv.h"
#include "date.h"

class Measurement {
 public:

  //Constructor
  Measurement(date::year_month_day date, std::string time, double temperature);

  //Get functions
  double get_temperature() const { return temperature; }
  std::string get_time() const { return time; }
  date::year_month_day get_date() const { return date; }

 private:
  date::year_month_day date;
  std::string time;
  double temperature;
};

#endif /* MEASUREMENT_H */