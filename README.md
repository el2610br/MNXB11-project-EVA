### MNXB11-project-EVA (Ellen, Verena & Andreas)
## About
It is for the project done in MNXB11 in 2024 by the lovely group EVA (Ellen Brandt Sahlberg, Verena Hehl and Andreas Pettersson)!

## Overview

During this project, we analyzed several temperature measurement datasets from "SMHI - Sveriges meteorologiska och hydrologiska institut". Three analyses were performed. The first one calculated the mean temperature and standard deviation for each day for all the years in dataset. The second one finds the warmest and coldest days in each year and sums them up in one histogram. It also includes a Gaussian fit of the warmest days. The third analysis counts the days above 25 degree Celcius in each year, counts them up and then plots the count of the number of days against all the years. 

## Build instructions

# Build and install the CLI library
The following commands should be executed from the git root directory.

```
# Clone the project "HTTPS LINK TO OUR PROJECT"
# cd into the repository
cd MNXB11-project-EVA

# Download the external library
git clone https://github.com/adishavit/argh.git

# Create a directory for the CLI header
mkdir -pv external/include/

# Copy the header file into the new directory
cp argh/argh.h external/include/

# Done!
```

# Build and install the CSV library
```
# cd into the repository
cd MNXB11-project-EVA

# Download the external library 
# We chose the fast-cpp-csv-parser
git clone https://github.com/ben-strasser/fast-cpp-csv-parser.git

# If the directory hasn't been created while building the CLI library, then create a directory for the CSV header
mkdir -pv external/include/

# Copy the header file into the directory
cp fast-cpp-csv-parser/csv.h external/include/

# Done!
```

# Build and install the fmt library
```
# cd into the repository
cd MNXB11-project-EVA

# Create a folder called "dependencies" and enter it, to store our library
mkdir dependencies
cd dependencies

# Download the fmt library
git clone https://github.com/fmtlib/fmt.git

# If the directory hasn't been created while building the CLI library, then create a directory for the CSV header, inside the MNXB11-project-EVA directory
mkdir -pv external/include/

# Also create a directory called "build" in the MNXB11-project-EVA 
# Create a directory called fmt inside of it
mkdir -pv build/fmt

# Enter the build/fmt directory
cd build/fmt

# Run the configuration step, specifying your username
cmake /home/username/git/MNXB11-project-EVA/dependencies/fmt -DCMAKE_INSTALL_PREFIX=~/git/MNXB11-project-EVA/external

# To build the library, run "make" and "make install"
make
make install

#Done!

```

# Build and install the Date library
```
# cd into the correct repository
cd MNXB11-project-EVA

# Download the date library
git clone https://github.com/HowardHinnant/date.git

# If the directory hasn't been created while building the CLI library and CSV library, then create a directory for the date header
mkdir -pv external/include/

# Copy the header file into the directory
cp ~/git/MNXB11-project-EVA/dependencies/date/include/date/date.h ~/git/MNXB11-project-EVA/external/include/


# Now it's installed!

```

# Build the project

```
# In the project directory, make the project
make
# Get help from the CLI library 
./main --help

```


# Directory structure

The final folder structure should look like this:
- datasets
  - README.md: information on how to download datasets
  - smhicleaner.sh: code to clean data

- include: contains all header files
  - Analysis.h
  - DataExtraction.h
  - Measurement.h

- src
  - Analysis.cxx: code where the three analyses are performed
  - DataExtraction.cxx: extracting and cleaning data
  - Measurement.cxx

- .gitignore: will ignore certain files when adding to your repository

- AUTHORS

- ChangeLog: document keeping track of all changes performed by the group members

- Makefile: initializes the main file by including error flags and such

- README.md: this file!

- main.cxx: runs the program

- workplan.md: all the work that needed to be done


# How to run the project

In the main.cxx file, four different cases have been defined. Case 0 runs all of the three analyses. Case 1 does the calculation of the mean temperature over a year, Case 2 does the warmest and hottest days and Case 3 runs all the days that are above 25 degree Celcius. To run the code, the following needs to be done

```
# To run the Makefile
make

# To run the Analysis
./main -i="filename" -a="Case"

# An example for Boras and Analysis 1
./main -i=smhi-opendata_1_72450_20231007_155620_Boras.csv -a=1

# To clean the folders
make clean

```
