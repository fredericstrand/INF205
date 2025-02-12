binary = LJTS
folder = assignment_2

$(binary): molecule.0 readxyz.o main.o
	g++ -O3 -o $@ $^

clean:
	rm -f $(binary) *.o *.zip

zip: clean
	zip $(folder) Makefile *.cpp
