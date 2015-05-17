all: hash

hash: main.o hash.o 
	g++ main.o hash.o -o pruebaDeHash

main.o: main.cpp
	g++ -c main.cpp

hash.o: hash.cpp hash.h
	g++ -c hash.cpp

clean:
	rm *o pruebaDeHash

