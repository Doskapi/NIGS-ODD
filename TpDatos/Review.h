
#ifndef REVIEW_H_
#define REVIEW_H_

//Inclusiones necesarias
#include <fstream>
#include <iostream>
using namespace std;

class Review {
private:
	  string id;
	  string body;

	  void eliminarCodigoHtml();

	  void eliminarMayusculas();

public:

	Review();

	Review(string unReview);

	Review(string unId , string unCuerpo );

	string getId();

	string getBody();

	virtual ~Review();
};

#endif /* REVIEW_H_ */
