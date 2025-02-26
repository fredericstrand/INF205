CXX = g++
CXXFLAGS = -O2 -std=c++11 -Wall -Wextra
TARGET1 = readxyz
TARGET2 = genxyz
OBJS1 = main.o readxyz.o molecule.o
OBJS2 = genxyz.o molecule.o

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(OBJS1)

$(TARGET2): $(OBJS2)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(OBJS2)

main.o: main.cpp molecule.h
	$(CXX) $(CXXFLAGS) -c main.cpp

genxyz.o: genxyz.cpp molecule.h
	$(CXX) $(CXXFLAGS) -c genxyz.cpp

readxyz.o: readxyz.cpp
	$(CXX) $(CXXFLAGS) -c readxyz.cpp

molecule.o: molecule.cpp molecule.h
	$(CXX) $(CXXFLAGS) -c molecule.cpp

clean:
	rm -f *.o $(TARGET1) $(TARGET2)
