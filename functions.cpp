#include <iostream>
#include <map>
#include "functions.hpp"

using std::cout; using std::endl; using std::string; using std::map;

string get_moyenne(string http_response){
	return "Moyenne : "+http_response;
}

map<char, Callback> callbacks = {
	{'8', get_moyenne}
};

void executeCallback(char key, string http_response, int lcdhd){
	lcdPosition(lcdhd, 0, 1);
	lcdPrintf(lcdhd, callbacks.at(key)(http_response).c_str());
}

bool callback_exists(char key){
	return callbacks.find(key) != callbacks.end();
}

