
### Workplan

## Structure

# 0. main.cxx
Here we have the options for the program. One option should be what kind of analysis that should be run.

# 1. Data extraction

First clean the data with smhicleaner. Create a new .sh file that calls smhicleaner to make a new "cleaned" file and then send back the name of that new file. Change in SMHI cleaner so that the new file is stored in the correct location and that we get back the name.

First the data needs to be reformatted so that we can analyse it. This is done with CLI & CSV parsers. This data is then saved in a new TTree file that we can access in analysis.

DataExtraction.cxx, DataExtraction.h

Functions in DataExtraction.h:

read_measurements:
Extracts the data and then stores the data in a vector.

persist_measurements:
Takes the vector data and saves in in a TTree. This means we have a TTree that we can then access with ROOT to create plots.


# 2. Analysis
We analyse the new CSV files in order to obtain the result that we are looking for. 
We take the measurement data and stores it in histograms and saves in a output file.

We want three analyses with their respective plots:

1. The temperature of every day of a year

2. The warmest and coldest day of each year

3. The number of days with temperatures above 25 degrees in a year for each year

# 3. Plotting
We use ROOT to plot figures.
