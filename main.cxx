
#include <iostream>
#include <string>
#include "argh.h"

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

//Just testing stuff here. Cmdl includes the CLI arguments.
//cmdl[0] would give ./main in this case. cmdl[1] would print the first parameter.
//We use cmdl to find the options that we want for the executable, e.g. if we want a commmand that makes the executable
//not run a certain part of the program, we can call this -norun and use an if statement to only run a certain
//part of the program. EASY!

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
  }

  return 0;
}
