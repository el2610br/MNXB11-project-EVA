#include "Analysis.h"
#include "Measurement.h"

//#include <TFile.h>
//#include <TH1D.h>
#include <TCanvas.h>


//Creating a histogram to plot the mean temperature over a year
void mean_temp_over_a_year(const std::vector<Measurement>& data, const std::string& output_filename) {

  TFile output_file{output_filename.c_str(), "RECREATE"};
  
  TH1D* meantemp_histogram{new TH1D{"meantemp_histogram", "Mean Daily Temperature over a Year;Day of Year;Temperature [°C]", 365, 0, 366}};

  //Create arrays to store data
  std::vector<double> daily_temp_sum(365, 0); //to store the sum of the temps each day
  std::vector<int> daily_temp_count(365,0); //the number of temperature measurements for each day
  //we need to divide our daily_temp_sum by the daily_temp_count to get the mean temperature
  
  std::vector<int> daily_temp_sum_sq(365, 0);

  for (const auto& measurement : data) {
    
    //conversion of "YYYY-MM-DD" to numbers from 1 to 365 (does not account for leap years, I think)
    std::string date_str = date::format("%F", measurement.get_date());
    std::istringstream in(date_str.c_str());
    date::year_month_day ymd;
    in >> date::parse("%F", ymd);
    int day_of_year = (date::sys_days{ymd} - date::sys_days{date::year{ymd.year()}/1/1}).count();
    
    //Check that the date is valid
    if (day_of_year < 0 || day_of_year > 365) {
      std::cerr << "Invalid date: " << day_of_year << std::endl;
      continue;
    }

    //Check that the temp is valid
    if (measurement.get_temperature() < -50 || measurement.get_temperature() > 50) {
      std::cerr << "Invalid temperature data: " << measurement.get_temperature() << std::endl;
      continue;
    }    
    
    double temp = measurement.get_temperature();  
    daily_temp_sum[day_of_year] += temp;
    daily_temp_sum_sq[day_of_year] += temp * temp;
    daily_temp_count[day_of_year] += 1;     
  

  for (int day = 0; day < 365; ++day) {
    if (daily_temp_count[day] > 0) {
      double mean_temperature = daily_temp_sum[day] / daily_temp_count[day];
      double mean_temp_sq = daily_temp_sum_sq[day] / daily_temp_count[day];
      double variance = mean_temp_sq - mean_temperature * mean_temperature;
      double stddev = (variance > 0) ? std::sqrt(variance) : 0;

      meantemp_histogram->SetBinContent(day + 1, mean_temperature); // +1 to match ROOT's 1-based bin indexing
      meantemp_histogram->SetBinError(day + 1, stddev); // Set standard deviation as the error    
    }
  }
  }
  
  TCanvas* canvas = new TCanvas("mean_temp_canvas", "Mean temperature over a year", 800, 600);
  meantemp_histogram->Draw();

  canvas->SaveAs("mean_temperature_over_a_year.png"); // Save as PNG

  //Clean up
  delete canvas;

  //Closes the output file
  //output_file.Write();
  output_file.Close();
  
}




void fill_histogram_with_vector(TH1D* histogram, const std::vector<double>& data_vector) {
    for (double value : data_vector) {
        histogram->Fill(value); // Fill histogram with each value from the vector
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void warmest_coldest_over_a_year(const std::vector<Measurement>& data, const std::string& output_filename) {
  
  TFile output_file{output_filename.c_str(), "RECREATE"};

  //Create the histograms
  TH1D* warmest_days_histogram = new TH1D{"warmest_days_histogram", "Count of Warmest Days;Day of Year;Count", 365, 0, 365};
  TH1D* coldest_days_histogram = new TH1D{"coldest_days_histogram", "Count of Coldest Days;Day of Year;Count", 365, 0, 365};

  //Set colors for filling later
  warmest_days_histogram->SetFillColor(kRed);
  warmest_days_histogram->SetLineColor(kRed);  // Set line color to red (optional)
  warmest_days_histogram->SetLineWidth(0);      // Remove outline (set line width to 0)
  coldest_days_histogram->SetFillColor(kBlue);
  coldest_days_histogram->SetLineColor(kRed);  // Set line color to red (optional)
  coldest_days_histogram->SetLineWidth(0);      // Remove outline (set line width to 0)

  //Create maps to store data
  std::unordered_map<std::string, double> warmest_days_val;
  std::unordered_map<std::string, double> coldest_days_val;
  std::unordered_map<std::string, std::unordered_map<int, int>> warmest_days_count;
  std::unordered_map<std::string, std::unordered_map<int, int>> coldest_days_count;

  //For-loop through the data
  for (const auto& measurement : data) {

    //conversion of "YYYY-MM-DD" to numbers from 1 to 365
    std::string date_str = date::format("%F", measurement.get_date());
    std::istringstream in(date_str.c_str());
    date::year_month_day ymd;
    in >> date::parse("%F", ymd);
    int day_of_year = (date::sys_days{ymd} - date::sys_days{date::year{ymd.year()}/1/1}).count();

    //Convert the year to a string
    std::string year = std::to_string(static_cast<int>(ymd.year()));

    //Check that the date is valid
    if (day_of_year < 0 || day_of_year > 365) {
      std::cerr << "Invalid date: " << day_of_year << std::endl;
      continue;
    }

    //Check that the temp is valid
    if (measurement.get_temperature() < -50 || measurement.get_temperature() > 50) {
      std::cerr << "Invalid temperature data: " << measurement.get_temperature() << std::endl;
      continue;
    }

    //We want to save only the coldest and warmest day of each year
    //We check if we have saved a value for this day and then if the temp is warmer or not than the saved value
    if (warmest_days_val.count(year) > 0) {
      double old_value = warmest_days_val[year];
      if (old_value > measurement.get_temperature()) {
      }else{
        warmest_days_val[year] = measurement.get_temperature();
        warmest_days_count[year].clear();
        warmest_days_count[year][day_of_year] = 1;
      }
    }else{
      warmest_days_val[year] = measurement.get_temperature();
      warmest_days_count[year].clear();
      warmest_days_count[year][day_of_year] = 1;
    }

    //Same thing for the coldest days
    if (coldest_days_val[year] != 0) {
      double old_value = coldest_days_val[year];
      if (old_value < measurement.get_temperature()) {
      }else{
        coldest_days_val[year] = measurement.get_temperature();
        coldest_days_count[year].clear();
        coldest_days_count[year][day_of_year] = 1;
      }
    }else{
      coldest_days_val[year] = measurement.get_temperature();
      coldest_days_count[year].clear();
      coldest_days_count[year][day_of_year] = 1;
    }

  }
  
  //Fill the histograms
  for (const auto& year_pair : warmest_days_count) {
    for (const auto& day_pair : year_pair.second) {
        warmest_days_histogram->Fill(day_pair.first);
    }
  }
  for (const auto& year_pair : coldest_days_count) {
    for (const auto& day_pair : year_pair.second) {
        coldest_days_histogram->Fill(day_pair.first);
    }
  }

  //Draw the histograms
  TCanvas* canvas = new TCanvas("warmest_coldest_canvas", "Warmest and Coldest days Histogram", 800, 600);
  warmest_days_histogram->Draw("HIST SAME"); // Draw warmest days histogram
  coldest_days_histogram->Draw("HIST SAME"); // Draw coldest days histogram
  canvas->SaveAs("warmest_coldest_histogram.png"); // Save as PNG

  //Clean up
  delete canvas;

  //Close the output file
  output_file.Close();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// How many warm days (>25 degrees) there are in a year over the years, to she if the temperatures have increased over the years 


void days_above_25_degrees(const std::vector<Measurement>& data, const std::string& output_filename) {

    TFile output_file{output_filename.c_str(), "RECREATE"};

    // Here we are creating a map to store the number of days above 25 degrees for each year
    std::unordered_map<int, int> days_above_25_per_year;
    // This is a set that will help us only adding one count per day
    std::unordered_set<std::string> processed_dates;

    // This is a For-loop that loops through all the data in measurement
    for (const auto& measurement : data) {

        // The convertion "YYYY-MM-DD" date a string
        std::string date_str = date::format("%F", measurement.get_date());
        std::istringstream in(date_str.c_str());
        date::year_month_day ymd;
        in >> date::parse("%F", ymd);

        // Creates an integer of the year
        int year = static_cast<int>(ymd.year());

        // Seeing if the temperature is above 25 and check if the date has been counted towards the number of days yet 
        // .end() is just way of saying not found
        if (measurement.get_temperature() > 25 && processed_dates.find(date_str) == processed_dates.end()) {
            // Add the date to the set 
            processed_dates.insert(date_str);

            // std::cout << date_str <<std::endl;

            // Adding the count to our map
            days_above_25_per_year[year]++;
        }
    }

    // Determine the range of years for the histogram x-axis

    // Integer placeholder for the minimum and maximum
    int min_year = days_above_25_per_year.begin()->first;
    int max_year = days_above_25_per_year.begin()->first;

    // Here we are looping through the years to find the minimum/maximum and adding it to our integers
    for (const auto& year_temp_pair : days_above_25_per_year) {
        min_year = std::min(min_year, year_temp_pair.first);
        max_year = std::max(max_year, year_temp_pair.first);
    }

    // Finding the range of years
    int num_years = max_year - min_year + 2;

    // Creating the histogram for the number of days above 25 degrees per year
    TH1D* days_above_25_histogram = new TH1D{"days_above_25_histogram", "Days Above 25 Degrees Per Year;Year;Number of days", num_years, min_year - 1.0, max_year + 1.0};

    // Fill the histogram with the number of days for each year
    for (const auto& year_temp_pair : days_above_25_per_year) {
        // .first is the key so our year
        int year = year_temp_pair.first;
        // .second is the value in the map corresponding to the key, so the number of days over 25 degrees
        int count = year_temp_pair.second;
        // This is filling the histogram
        days_above_25_histogram->Fill(year, count);
    }


    // Draw the histogram
    TCanvas* canvas = new TCanvas("above_25_per_year_canvas", "Days Above 25 Degrees Per Year Histogram", 800, 600);
    days_above_25_histogram->SetFillColor(kPink-8);
    days_above_25_histogram->SetLineColor(kPink-8);
    days_above_25_histogram->SetStats(0);
    days_above_25_histogram->Draw("HIST");
    canvas->SaveAs("days_above_25_per_year_histogram.png"); // Save as PNG

    // Clean up
    delete canvas;

    // Close the output file
    output_file.Close();
}