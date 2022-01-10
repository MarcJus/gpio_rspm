#include <iostream>
#include <map>
#include <sstream>
#include "functions.hpp"

using std::cout; using std::endl; using std::string; using std::map;

string get_moyenne(string http_response){
	return "Moyenne : "+http_response;
}

string nombre_devoirs(string http_response){
	std::istringstream stream(http_response);
	string line;
	string devoirs;
	while(std::getline(stream, line)){
		devoirs = line;
		break;
	}
	return devoirs+" devoirs";
}

/*
 * Liste des fonctions pour chaque clé
 */
map<char, Callback> callbacks = {
	{'5', nombre_devoirs},
	{'8', get_moyenne}
};

void executeCallback(char key, string http_response, int lcdhd){
	lcdPosition(lcdhd, 0, 1);
	lcdPrintf(lcdhd, callbacks.at(key)(http_response).c_str());
}

bool callback_exists(char key){
	return callbacks.find(key) != callbacks.end();
}

