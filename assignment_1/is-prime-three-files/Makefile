binary = is-prime-900
folder = is-prime-three-files

$(binary): only-is-prime.o only-main.o
	g++ -o $@ $^

clean:
	rm -f $(binary) *.o *.zip

zip: clean
	zip $(folder) Makefile *.cpp
