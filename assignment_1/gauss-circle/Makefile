# Compiler and flags
CXX = g++
CXXFLAGS = -I. -Wall

# Variables
binary = gauss_circle
sources = main.cpp is-gauss.cpp
objects = $(sources:.cpp=.o)

# Default target
all: $(binary)

# Build the binary
$(binary): $(objects)
	$(CXX) -o $@ $^

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(binary) *.o
