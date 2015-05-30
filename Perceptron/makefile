all: perceptron

perceptron: main.o funciones.o
	g++ main.o funciones.o -o perceptron

main.o: main.cpp
	g++ -c main.cpp

funciones.o: funciones.cpp funciones.h
	g++ -c funciones.cpp

clean:
	rm *.o perceptron

