all: bayes

bayes: bayes.o 
	g++ bayes.o -o bayes

bayes.o: bayes.cpp
	g++ -c bayes.cpp

clean:
	rm *.o bayes

