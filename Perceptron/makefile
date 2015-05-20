all: perceptron

perceptron: main.o archivos.o funciones.o
	g++ main.o archivos.o funciones.o -o perceptron

main.o: main.cpp
	g++ -c main.cpp

archivos.o: archivos.cpp archivos.h
	g++ -c archivos.cpp

funciones.o: funciones.cpp funciones.h
	g++ -c funciones.cpp

clean:
	rm *.o perceptron

