CXX := g++
CXXWARNINGS := -Wall -Wextra -Werror
CXXOPT := -O3
CXXSTD := -std=c++17
INCLUDES := -I include -I external/include/ -I `root-config --incdir`
CXXFLAGS := $(CXXWARNINGS) $(CXXSTD) $(CXXOPT) $(INCLUDES)

LDFLAGS := -Wno-stringop-truncation -Lexternal/lib64 -lfmt `root-config --glibs` -lstdc++
STRNGOP := -Wno-stringop-truncation

.PHONY: all clean

all: main

# If you add new source files in the src/ directory, remember to add the
# corresponding object file as a dependency here so that Make knows that it
# should build it and link to it
#
# Remove the Example object file when you are done looking at it, it doesn't
# contribute to the executable!

main: main.cxx src/Measurement.o src/Analysis.o src/DataExtraction.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(STRNGOP)

clean:
	rm -v src/*.o main
