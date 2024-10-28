#include "Analysis.h"
#include "date.h"
#include <iostream>

//Creating a histogram to plot the mean temperature over a year
void mean_temp_over_a_year(const std::vector<Measurement>& measurements,
                           const std::string& output_filename) {
  // Note: We need .c_str() because TFile expects a C-string (i.e. a char*)
  // but output_filename is a std::string
  // Note: We use UPDATE here rather than RECREATE so that we can add new
  // histograms if we already created the file with the raw data. 
  TFile output_file{output_filename.c_str(), "UPDATE"};
  TH1D* meantemp_histogram{new TH1D{"meantemp_histogram", "Mean Daily Temperature over a Year;Day of Year;Temperature [°C]", 365, 0, 366}};

  //Create arrays to store data
  std::vector<double> daily_temp_sum(365, 0); //to store the sum of the temps each day
  std::vector<int> daily_temp_count(365,0); //the number of temperature measurements for each day
  //we need to divide our daily_temp_sum by the daily_temp_count to get the mean temperature
  
  std::vector<int> daily_temp_sum_sq(365, 0);

  for (const auto& measurement : measurements) {
    //conversion of "YYYY-MM-DD" to numbers from 1 to 365 (does not account for leap years, I think)
    std::istringstream in{measurement.get_date()};
    date::year_month_day ymd;
    in >> date::parse("%F", ymd);
    auto day_of_year = (date::sys_days{ymd} - date::sys_days{date::year{ymd.year()}/1/1}).count();
    
    //daily_temp_sum[day_of_year] += measurement.get_temperature();
    //daily_temp_count[day_of_year] += 1;

    //date_histogram->Fill(measurement.get_date());
    //date_histogram->Fill(day_of_year +1)
    //temperature_histogram->Fill(measurement.get_temperature());

    if (day_of_year >= 0 && day_of_year < 365) {
      double temp = measurement.get_temperature();  
      daily_temp_sum[day_of_year] += temp;
      daily_temp_sum_sq[day_of_year] += temp * temp;
      daily_temp_count[day_of_year] += 1;     
    }

  for (int day = 0; day < 365; ++day) {
    if (daily_temp_count[day] > 0) {
      //double mean_temperature = daily_temp_sum[day] / daily_temp_count[day];
      //meantemp_histogram->SetBinContent(day + 1, mean_temperature);  // day + 1 for 1-based day-of-year
      double mean_temperature = daily_temp_sum[day] / daily_temp_count[day];
      double mean_temp_sq = daily_temp_sum_sq[day] / daily_temp_count[day];
      double variance = mean_temp_sq - mean_temperature * mean_temperature;
      double stddev = (variance > 0) ? std::sqrt(variance) : 0;

      meantemp_histogram->SetBinContent(day + 1, mean_temperature); // +1 to match ROOT's 1-based bin indexing
      meantemp_histogram->SetBinError(day + 1, stddev); // Set standard deviation as the error    
    }
  }

  //customize the histogram still

  meantemp_histogram->Write();

  //Closes the output file
  output_file.Close();
}



// Note: We aren't giving a name to the parameter here, this tells the compiler
// that we know the parameters aren't used
void some_analysis(const std::vector<Measurement>& /*measurements*/,
                   const std::string& /*output_filename*/) {
  std::cout << "Here we could implement the code for a second analysis!"
            << std::endl;
  std::cout << "You could consider using a different translation unit for this "
               "to make it easier to work on the different analyzes "
               "independently in git!"
            << std::endl;
}
void some_other_analysis(const std::vector<Measurement>& /*measurements*/,
                         const std::string& /*output_filename*/) {
  std::cout << "Here we could implement the code for a third analysis!"
            << std::endl;
  std::cout << "You could consider using a different translation unit for this "
               "to make it easier to work on the different analyzes "
               "independently in git!"
            << std::endl;
}