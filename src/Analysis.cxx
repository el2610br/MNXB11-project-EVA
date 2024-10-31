#include "Analysis.h"
#include "Measurement.h"

//#include <TFile.h>
//#include <TH1D.h>
#include <TCanvas.h>

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cmath>
#include <TF1.h>
#include <TMath.h>
#include <TLegend.h>


//Creating a histogram to plot the mean temperature over a year
void mean_temp_over_a_year(const std::vector<Measurement>& data, const std::string& output_filename) {

  TFile output_file{output_filename.c_str(), "RECREATE"};
  
  TH1D* meantemp_histogram{new TH1D{"meantemp_histogram", "Mean Daily Temperature over a Year;Day of Year;Temperature [#circC]", 365, 0, 366}};

  //Create vectors to store data
  std::vector<double> daily_temp_sum(365, 0); //to store the sum of the temps each day
  std::vector<int> daily_temp_count(365,0); //the number of temperature measurements for each day
  //we need to divide our daily_temp_sum by the daily_temp_count to get the mean temperature
  
  std::vector<int> daily_temp_sum_sq(365, 0); //to calculate the 

  for (const auto& measurement : data) {
    
    //conversion of "YYYY-MM-DD" to numbers from 1 to 365 for the days
    std::string date_str = date::format("%F", measurement.get_date()); //converts date into a string
    std::istringstream in(date_str.c_str()); //converts string to C-style string, which istringstream takes as input
    date::year_month_day ymd; //year_month_day is from date library, it will save it as year, month and day separately
    in >> date::parse("%F", ymd); //parses the input stream "in" into ymd
    //if we had before "2024-10-01" then ymd will be "year=2024", "month=10", "day=01"

    //this is the actual calculation to go from 1 to 365
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
    
    // initialize the temperature values and add them to the created vectors
    double temp = measurement.get_temperature();  
    daily_temp_sum[day_of_year] += temp; //for each day of the year it adds all the temperatures
    daily_temp_sum_sq[day_of_year] += temp * temp;
    daily_temp_count[day_of_year] += 1;     
  

  for (int day = 0; day < 365; ++day) {
    if (daily_temp_count[day] > 0) {
      //compute the means by dividing by the count
      double mean_temperature = daily_temp_sum[day] / daily_temp_count[day];
      double mean_temp_sq = daily_temp_sum_sq[day] / daily_temp_count[day];
      //variance and standard deviation
      double variance = mean_temp_sq - mean_temperature * mean_temperature;
      double stddev = (variance > 0) ? std::sqrt(variance) : 0; //only computes the standard deviation if variance bigger than zero

      meantemp_histogram->SetBinContent(day + 1, mean_temperature); // bin counting starts at 1 in root
      meantemp_histogram->SetBinError(day + 1, stddev); // standard deviation is the bin error for each bin    
    }
  }
  }

  // Customize histogram colors
  meantemp_histogram->SetLineColor(kCyan-2); // Set mean temperature line color to red
  meantemp_histogram->SetMarkerColor(kBlue-2); // Set marker color for the mean temperature
  meantemp_histogram->SetMarkerStyle(21); // Set marker style
  meantemp_histogram->SetMarkerSize(0.8);
  
  meantemp_histogram->SetStats(0); // Disable the statistics box
  
  //initializes the canvas to put the plot onto
  TCanvas* canvas = new TCanvas("mean_temp_canvas", "Mean temperature over a year", 800, 600);
  meantemp_histogram->Draw();

  // Save the histogram as png
  canvas->SaveAs("mean_temperature_over_a_year.png"); 

  //Clean up
  delete canvas;

  //Closes the output file
  output_file.Close();
  
}

//Analysis 2
// Define the Gaussian function 
double Gaussian(double* x, double* par) {
  return par[0] * TMath::Exp(-0.5 * (x[0] - par[1]) * (x[0] - par[1]) / (par[2] * par[2]));
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
  warmest_days_histogram->SetStats(0);
  coldest_days_histogram->SetStats(0);

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
  
  //Create histogram for the warm Gaussian
  TF1* warm_gaussian_fit = new TF1("Gaussian", Gaussian, 80, 320, 3);  // Gaussian fit function with parameters
  warm_gaussian_fit->SetParameters(20, 200, 100);  // Initial guesses for parameters (amplitude, mean, sigma) 
  warm_gaussian_fit->SetLineColor(kBlack);

  //Create histogram for the cold Gaussian
  // DOES NOT WORK CURRENTLY
  //TF1* cold_gaussian_fit_1 = new TF1("Gaussian", Gaussian, 300, 100, 3);  // Gaussian fit function with parameters
 
  //cold_gaussian_fit_1->SetParameters(20, 20, 100);  // Initial guesses for parameters (amplitude, mean, sigma)
  //cold_gaussian_fit_1->SetLineColor(kBlue);

  // Fit warmest days histogram with Gaussian and print results
  warmest_days_histogram->Fit(warm_gaussian_fit, "Q+");  // Fit without verbose output
  //coldest_days_histogram->Fit(cold_gaussian_fit_1, "Q+");
  std::cout << "The mean of the warmest days is " << warm_gaussian_fit->GetParameter(1) << std::endl;
  std::cout << "Its uncertainty of the warmest days is " << warm_gaussian_fit->GetParError(1) << std::endl;

  warm_gaussian_fit->Draw("HIST SAME");
  //cold_gaussian_fit_1->Draw("HIST SAME"); 

  //Create a legend
  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->SetTextSize(0.03);
  legend->AddEntry(warmest_days_histogram, "Warmest days", "f");
  legend->AddEntry(coldest_days_histogram, "Coldest days", "f");
  legend->AddEntry(warm_gaussian_fit, "Gaussian Fit", "l");
  legend->Draw();

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