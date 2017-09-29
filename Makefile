compile:
	g++ -c -o bin/grammar.o -std=c++0x src/grammar.cpp
	g++ -c -o bin/options.o -std=c++0x src/options.cpp
	g++ -c -o bin/globals.o -std=c++0x src/globals.cpp
	g++ -c -o bin/interpreter.o -std=c++0x src/interpreter.cpp
	g++ -c -o bin/main.o -std=c++0x src/main.cpp
link:
	g++ -o start bin/grammar.o bin/options.o bin/globals.o bin/interpreter.o bin/main.o
clean:
	rm -rf bin/*
	rm ./start
all:
	$(MAKE) clean
	$(MAKE) compile
	$(MAKE) link
