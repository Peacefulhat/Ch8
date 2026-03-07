LinkerFlags := -lraylib

chip8: chip-8.o core.o
	g++ chip-8.o core.o -o chip8 $(LinkerFlags)

chip-8.o: chip-8.cpp
	g++ -c chip-8.cpp

core.o: core.cpp
	g++ -c core.cpp

clean:
	rm -rf *.o
