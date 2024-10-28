#include "DataExtraction.h"
#include "date.h"

std::string cleaning(std::string file_name){
  const char* path = "~/git/MNXB11-project-EVA/datasets/smhicleaner.sh ~/git/MNXB11-project-EVA/datasets/";

  std::string combined = std::string(path) + file_name + " > /dev/null 2>&1";

  const char* cleaner_path = combined.c_str(); 

  system(cleaner_path);

  std::string new_string= "baredata_" + file_name;

  return new_string;
}

void directory_cleaning(std::string file_name){
  //Takes the paths of the new files created from cleaning() and removes them
  std::string path_baredata = ("rm " + file_name);

  std::string original = "original_" + file_name.substr(9,100);
  std::string path_original = ("rm " + original);

  system(path_baredata.c_str()); system(path_original.c_str());
}

std::vector<Measurement> read_csv(std::string file_name){

    //We declare the vector where we want our data
    std::vector<Measurement> measurements;

    //Run the csv parser on our file and assign it to in
    io::CSVReader<4> in(file_name);

    //Assign variables
    std::string date; std::string time; double temperature; std::string quality;
    int y; int m; int d;

    //Iterate through every row in our datafile for all four columns (date, time, temp, quality of measurement)
    while(in.read_row(date, time, temperature, quality)){
      //Take the date string and put it into integers for year, month and date and then make a new variable with the
      //date function
      y = std::stoi(date.substr(0,4)); m = std::stoi(date.substr(5,2)); d = std::stoi(date.substr(8,2));
      date::year_month_day date{date::year(y), date::month(m), date::day(d)};

      //Check that the given date is a valid date
      if (!date.ok()) {
        std::cerr << "Invalid date: " << y << "-" << m << "-" << d << std::endl;
        continue; 
      }

      //Declare a Measurement class and then add it to the vector
      Measurement mesh(date, time, temperature);
      measurements.push_back(mesh);

    }

  return measurements;
}

/*


std::vector<Measurement> read_measurements(const std::string& file_name) {
    // This code is WRONG! Can you spot the problems? It isn't easy, which is why
    // one of the reasons you don't want to write this kind of code yourself.
    std::ifstream file{file_name};
    std::vector<Measurement> measurements;
    // Declaring a bunch of variables in advance, not good practice! Use a library
    // for reading CSV!
    int year, month, day, hour, minute, second, id;
    double signal, background, value;
    std::string header;
    // Assume first line is a header, read it first before we get the actual data
    std::getline(file, header);

  for (const auto measurement : measurements) {
    id = measurement.get_id();
    background = measurement.get_background();
    signal = measurement.get_signal();
    tree.Fill();
  }
  tree.Write();
}

*/
