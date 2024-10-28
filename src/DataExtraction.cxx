#include "DataExtraction.h"
#include "Measurement.h"




std::string cleaning(std::string file_name){
  const char* path = "~/git/MNXB11-project-EVA/datasets/smhicleaner.sh ~/git/MNXB11-project-EVA/datasets/";

  std::string combined = std::string(path) + file_name + " > /dev/null 2>&1";

  const char* cleaner_path = combined.c_str(); 

  system(cleaner_path);

  std::string new_string= "baredata_" + file_name;

  return new_string;

}




int read_csv(std::string file_name){

    std::vector<Measurement> measurements;

    io::CSVReader<4> in(file_name);
    in.read_header(io::ignore_extra_column, "day", "year", "month", "measurement");

    int day; int year; int month; double measurement;

    while(in.read_row(day, year, month, measurement)){

        date::year_month_day date{date::year(year), date::month(month), date::day(day)};

        if (!date.ok()) {
            std::cerr << "Invalid date: " << year << "-" << month << "-" << day << std::endl;
            continue; 
        }

        std::cout << format("%F", date) << "," << measurement << std::endl;

      }

  return 0;

}

/*

std::vector<Measurement> read_measurements(const std::string& filename) {
  // This code is WRONG! Can you spot the problems? It isn't easy, which is why
  // one of the reasons you don't want to write this kind of code yourself.
  std::ifstream file{filename};
  std::vector<Measurement> measurements;
  // Declaring a bunch of variables in advance, not good practice! Use a library
  // for reading CSV!
  int year, month, day, hour, minute, second, id;
  double signal, background, value;
  std::string header;
  // Assume first line is a header, read it first before we get the actual data
  std::getline(file, header);

  char comma;

  while (file >> year >> comma >> month >> comma >> day >> comma >> hour >>
         comma >> minute >> comma >> second >> comma >> id >> comma >> signal >>
         comma >> background >> comma >> value) {
    Measurement measurement{signal, background, id};
    if (measurement.is_valid()) {
      measurements.push_back(measurement);
    } else {
      std::cerr << "Warning: Skipping invalid measurement from (signal, "
                   "background, id): ("
                << signal << ", " << background << ", " << id << ")"
                << std::endl;
    }

        

    return 0;

}

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
