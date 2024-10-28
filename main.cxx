#include "DataExtraction.h"

//Function that prints the help documentation after running the -h, --help option.

void print_help() {
  std::cout << "Usage: main [OPTION]...\n"
            << "Takes a file with option -i and performs operations on it.\n"
            << "\n"
            << "NOTE!\n"
            << "The options need an equal sign between the flag and parameter!\n"
            << "Example:\n"
            << "main -i=some_file.csv\n"
            << "\n"
            << "Options:\n"
            << "-h, --help        Show this message and exit\n"
            << "-i, --include     The file to be included\n";
}


int main(int, char* argv[]) {

  argh::parser cmdl(argv);

  const std::string program_name{cmdl[0]};
  std::string file_name;

  if (cmdl[{ "-h", "--help"}]) {
    print_help();
    return 0;
  }

  if (cmdl({ "-i", "--include"}) >> file_name) {
    std::cout << "The datafile is " << file_name << std::endl;

    file_name = cleaning(file_name);
    std::cout << "The file name: " << file_name << std::endl;

    read_csv(file_name);
  } 
  else {
    std::cout << "The executable requires a datafile. Please include -i=<your_file>" << std::endl;
    return 0;
  }

  int analysis_choice{};
  if (cmdl({ "-a", "--analysis"}) >> analysis_choice) {
    if (!(analysis_choice >= 0 && analysis_choice <= 3)) {
      std::cout << "The analysis parameter needs to be one of 0,1,2,3. Call --help for detailed description." << std::endl;
      }
    } 
  else {
    std::cout << "The executable requires an analysis choice. Please include -a=<n> where n = 0,1,2,3." << std::endl;
    return 0;
  }

  switch(analysis_choice) {
    case 0:
      // The case where we don't specify analysis
      break;
    case 1:
      // Analysis 1
      break;
    case 2:
      // Analysis 2
      break;
    case 3:
      // Analysis 3
      break;


  }

  return 0;
}
