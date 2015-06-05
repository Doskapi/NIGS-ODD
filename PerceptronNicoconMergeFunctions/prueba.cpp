#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

int main() {
    using namespace std;
    string sentece = "And I feel fine...";
    istringstream iss(sentence);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
    for (list<string>::iterator iterador=tokens.begin(); iterador != tokens.end(); ++iterador){
        cout << *iterador << "\n" << endl;
    }
    return 0;
}
