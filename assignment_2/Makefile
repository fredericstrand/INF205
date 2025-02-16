binary = readxyz
folder = assignment_2
CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wall -Wextra

$(binary): molecule.o readxyz.o main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(binary) *.o *.zip

zip: clean
	zip $(folder) Makefile *.cpp
