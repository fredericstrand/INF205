CXX = clang++
CXXFLAGS = -O2 -std=c++17 -DNDEBUG -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
LDFLAGS = -L/opt/homebrew/opt/libomp/lib -lomp
TARGET1 = readxyz
TARGET2 = genxyz
TARGET3 = heuristic
OBJS1 = main.o readxyz.o molecule.o molecularsystem.o
OBJS2 = genxyz.o molecule.o molecularsystem.o
OBJS3 = heuristic.o readxyz.o molecule.o molecularsystem.o

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(OBJS1)
	$(CXX) $(OBJS1) $(LDFLAGS) -o $(TARGET1)

$(TARGET2): $(OBJS2)
	$(CXX) $(OBJS2) $(LDFLAGS) -o $(TARGET2)

$(TARGET3): $(OBJS3)
	$(CXX) $(OBJS3) $(LDFLAGS) -o $(TARGET3)

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
