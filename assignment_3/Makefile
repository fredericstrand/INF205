CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra
TARGET1 = readxyz
TARGET2 = genxyz
TARGET3 = heuristic
OBJS1 = main.o readxyz.o molecule.o molecularsystem.o
OBJS2 = genxyz.o molecule.o molecularsystem.o
OBJS3 = heuristic.o readxyz.o molecule.o molecularsystem.o

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(OBJS1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(OBJS1)

$(TARGET2): $(OBJS2)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(OBJS2)

$(TARGET3): $(OBJS3)
	$(CXX) $(CXXFLAGS) -o $(TARGET3) $(OBJS3)

main.o: main.cpp molecule.h
	$(CXX) $(CXXFLAGS) -c main.cpp

genxyz.o: genxyz.cpp molecule.h molecularsystem.h
	$(CXX) $(CXXFLAGS) -c genxyz.cpp

readxyz.o: readxyz.cpp molecule.h
	$(CXX) $(CXXFLAGS) -c readxyz.cpp

heuristic.o: heuristic.cpp molecule.h molecularsystem.h
	$(CXX) $(CXXFLAGS) -c heuristic.cpp

molecule.o: molecule.cpp molecule.h
	$(CXX) $(CXXFLAGS) -c molecule.cpp

molecularsystem.o: molecularsystem.cpp molecularsystem.h
	$(CXX) $(CXXFLAGS) -c molecularsystem.cpp

clean:
	rm -f *.o $(TARGET1) $(TARGET2) $(TARGET3)