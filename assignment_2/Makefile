binary = LJTS
folder = assignment_2

$(binary): is-gauss.o main.o
	g++ -O3 -o $@ $^

clean:
	rm -f $(binary) *.o *.zip

zip: clean
	zip $(folder) Makefile *.cpp
