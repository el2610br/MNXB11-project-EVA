### MNXB11-project-EVA (Ellen, Verena & Andreas)
## About
It is for the project done in MNXB11 in 2024 by the lovely group EVA!

## Overview

After building the project, you can run the main executable that the Makefile produces. It takes a path to a CSV file as an argument and a choice of which analysis to run. Only one analysis is actually implemented but there's also an option to persist the raw data extracted from the CSV file. This isn't entirely necessary to do for a project of this size where reading the data takes little time but when working on huge datasets it is often a good idea to have a separate step for extracting the data you actually need from the rest.

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

# If the directory hasn't been created while building the CLI library, then create a directory for the CSV header
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
cp date/date.h external/include/

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

The base directory here contains your "main" file, i.e. the C++ file that you
will be using as your starting point in your project. The code in this file should ideally be short and just make use of the functionality that you've
prepared in your other translation units. The majority of your code should be placed in  the two folders where you define your translation units, `src/` and `include/`. 

Do remember to add your include guards to your header files, otherwise the compiler will
get multiple definitions if you end up loading two source files that include the
same header.

In the template you can find a small translation unit called Example (in [src/Example.cxx](src/Example.cxx) and [include/Example.h](include/Example.h)) that shows some commented reminders of how the syntax for some C++ constructs work. Feel free to use this as a reference to remind yourself of how to do something while working on it but make sure to remove it from your final project version!

There is a demonstration of a toy project you can use for inspiration at [EinarElen/MNXB11-project-demo](https://github.com/EinarElen/MNXB11-project-demo). You should not copy code from this repository. There are some intentional bugs hiding in there, see if you can spot them. 

We have also included three special files in the base of the repository 
- [.gitignore](.gitignore)
  - This file contains regular expressions that git tells git that it shouldn't add certain file to your repository. 
  - Your git repository should generally not contain binary files like object files or executables nor should it contain build artefacts like external libraries. 
- [.clang-format](.clang-format)
  - This file holds the configuration for the clang-format tool that you can use to format your code consistently 
  - It is a good idea to keep your code formatted in a consistent manner, especially when working in groups but doing it manually is a waste of your time. Use a tool for it!
  ```
  # Show what the src/Example.cxx file would look like if formatted
  clang-format src/Example.cxx 
  # Carry out the formatting in the file directly 
  clang-format src/Example.cxx -i
  ```
  - The `.clang-format` file holds the configuration that clang-format will use to determine how to format your code. By default, it will be formatted according to Google's style but you can pick any that you like from https://clang.llvm.org/docs/ClangFormatStyleOptions.html
- [rootlogon.C](rootlogon.C)
  - This file contains code that ROOT will execute automatically whenever you start it, a good place to place general style choices you want to make or anything else you always want to run! 
  - Be careful to not include anything that depends on your particular machine here (e.g. absolute paths)
# Building the project

The [datasets](datasets) folder contains open data from SMHI and a README.md with further information about it.


We have included a basic Makefile here which should be familiar to you. It follows the same project structure that we have been using in the course. When you add a new translation unit to the project, you have update the dependencies in the Makefile. 

By default, the `all` target will be run which 
- Compiles any `.cxx` files in the `src/` directory into object files 
- Compiles `main.cxx` and links with all the object files in `src/`

You can run the `clean` target to remove any object files that have been produced as well as the `main` executable.

## Adding external software libraries

If you want to make use of external software libraries with your project, you
will always have to tell the tool that builds your project. The Makefile included in this template will pick up any header files in the external/include directory and look for libraries in external/lib and external/lib64 so if you use external as your installation directory, you only need to add the corresponding `-l` flag to the linker.

Here's an illustration of the typical process to add a (CMake based) external library
``` sh
# Clone or download the library you want to use 
git clone https://somerepository.com/alibrary alibrary # The last argument determines what the directory will be called

mkdir build/alibrary -pv # -p will tell mkdir to create the build/ directory if it doesn't already exist 
# Go into the build directory
cd build/alibrary 
# Look up the documentation for the library to find out if there are any additional flags you need for CMake 

# This command tells CMake to configure the build directory based on the source code in the ../../alibrary folder and to install the resulting headers and library files into ../../external
cmake ../../alibrary -DCMAKE_INSTALL_PREFIX=../../external 
# Build and install! Use -jN to launch N jobs
make -j8 install # If you are on an 8-core machine
```

Make sure to document how to do this for any library you choose to use!

Have fun!
