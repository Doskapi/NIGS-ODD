
#include "Review.h"
using namespace std;


Review::Review() {
	this->id = "";
	this->body = "";
}

Review::Review(string unReview) {
	long int pos = unReview.find('\t');
	this->id = unReview.substr( 1 , (pos - 2) );
	this->body = unReview.substr( (pos + 2) , (unReview.size() - (pos + 3)) );

	eliminarCodigoHtml();
	eliminarMayusculas();

}

//Elimina el codigo html en el cuerpo del review
void Review::eliminarCodigoHtml() {
	while (body.find("<br />") != string::npos) {
		body = body.erase( body.find("<br />"), 6 );
	}
	while (body.find('\"') != string::npos) {
		body = body.erase( body.find('\"'), 2 );
	}
}

//Elimina las mayusculas en el cuerpo del Review
void Review::eliminarMayusculas() {
	for (string::size_type i = 0; i < body.length(); i++)
	{
	    body[i] = tolower (body[i]);
	}
}

//Contructor donde se le pasa manualmente el id y el cuerpo
Review::Review(string unId, string unCuerpo) {
	this->id = unId;
	this->body = unCuerpo;
}

//Devuelve el id del review
string Review::getId() {
	return this->id;
}

//Devuelve el cuerpo del review
string Review::getBody(){
	return this->body;
}

Review::~Review() {
	// TODO Auto-generated destructor stub
}

