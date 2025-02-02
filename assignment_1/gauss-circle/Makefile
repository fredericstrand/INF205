# Variables
binary = gauss_circle
sources = main.cpp num_points.cpp
objects = $(sources:.cpp=.o)
CXX = g++
CXXFLAGS = -I.

# Default target
all: $(binary)

# Build the binary
$(binary): main.o
	$(CXX) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(binary) *.o
