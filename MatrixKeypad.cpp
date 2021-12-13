/**********************************************************************
* Filename    : MatrixKeypad.cpp
* Description : Obtain the key code of 4x4 Matrix Keypad
* Author      : www.freenove.com
* modification: 2019/12/27
**********************************************************************/
#include "Keypad.hpp"
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "socket.h"

#define RED_LED 	21
#define GREEN_LED	26
#define REPLY_LENGHT	10
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {  //key code
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {1, 4, 5, 6 }; //define the row pins for the keypad
byte colPins[COLS] = {12,3, 2, 0 }; //define the column pins for the keypad
char code[4];
char code_secret[4] = {'4','3','8','7'};
byte my_index = 0;

//create Keypad object
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

using namespace std;

bool get_ip(string config_file, string *ip);

int main(int argc, char* argv[]){
    printf("Program is starting ... \n");
    string config_file = "config.conf";
    string address;
	if(!get_ip(config_file, &address)){
		cerr << "Erreur lors de l'ouverture du fichier" << endl;
		exit(EXIT_FAILURE);
	}
	
    wiringPiSetup();
    bool useLed = true;
    if(argc > 1){
	if(argv[0] == "--no-led"){
	    cout << "Use led : false" << endl;
	    useLed = false;
	}
    }

    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    
    char key = 0;
    keypad.setDebounceTime(50);
    while(1){
	digitalWrite(RED_LED, LOW);
        key = keypad.getKey();  //get the state of keys
        if (key){       //if a key is pressed, print out its key code
            printf("You Pressed key :  %c \n",key);
	    SOCKADDR_IN sin = getAddress(address.c_str(), 3000);
	    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	    if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR){
		cout << "Connexion réussie" << endl;
	    } else {
		cerr << "Connexion echouée à " << address << endl;
		
	    }
	    
	    char message[2] = {key, '\0'};
	    char reply[REPLY_LENGHT];
		    
	    bool send = sendMessage(message, sock, reply);
	    if(useLed){
		if(send){
		    digitalWrite(GREEN_LED, HIGH);
		    delay(750);
		    digitalWrite(GREEN_LED, LOW);
		} else {
		    for(int i = 0; i < 4; i++){
			digitalWrite(RED_LED, HIGH);
			delay(200);
			digitalWrite(RED_LED, LOW);
			delay(200);
		    }
		}
	    }
	    close(sock);
	    
        } else 
		continue;

	/*code[my_index] = key;
	if (my_index < 3)
	  my_index++;
	else
	{
	  if (memcmp(code, code_secret, 4) == 0)
	  {
	     printf("c'est le bon code\n");
	     digitalWrite(LED_GREEN, HIGH);
	     delay(3000);
	     digitalWrite(LED_GREEN, LOW);
	     
	  }
	  else
	  {
	     printf("c'est le mauvais code\n");
	     for(int i = 0; i < 4; i++){
		digitalWrite(LED_RED, HIGH);
		delay(200);
		digitalWrite(LED_RED, LOW);
		delay(200);
	     }
	  }
	  my_index = 0;
	}*/
    }
    return 1;
}

bool get_ip(string config_file, string *ip){
	fstream cFile(config_file);
    if(cFile.is_open()){
        string line;
        while(getline(cFile, line)){
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPosition = line.find("=");
            auto name = line.substr(0, delimiterPosition);
            auto value = line.substr(delimiterPosition + 1);
            if(name == "ip"){
                *ip = value;
            }
        }
    } else {
        return false;
    }
    return true;
}    
