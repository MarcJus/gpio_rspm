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
#include <wiringPiI2C.h>
#include <pcf8574.h>
#include <lcd.h>

#include "socket.h"
#include "http_request.hpp"
#include "functions.hpp"

#define RED_LED 	21
#define GREEN_LED	26
#define REPLY_LENGHT	10

int pcf8574_address = 0x27;        // PCF8574T:0x27, PCF8574AT:0x3F
#define BASE 64         // BASE any number above 64
//Define the output pins of the PCF8574, which are directly connected to the LCD1602 pin.
#define RS      BASE+0
#define RW      BASE+1
#define EN      BASE+2
#define LED     BASE+3
#define D4      BASE+4
#define D5      BASE+5
#define D6      BASE+6
#define D7      BASE+7

int lcdhd;// used to handle LCD

void printDataTime(){//used to print system time
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);// get system time
    timeinfo = localtime(&rawtime);//convert to local time
    printf("%s \n",asctime(timeinfo));
    lcdPosition(lcdhd,0,1);// set the LCD cursor position to (0,1)
    lcdPrintf(lcdhd,"Time:%02d:%02d:%02d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec); //Display system time on LCD
}
int detectI2C(int addr){
    int _fd = wiringPiI2CSetup (addr);
    if (_fd < 0){
        printf("Error address : 0x%x \n",addr);
        return 0 ;
    }
    else{
        if(wiringPiI2CWrite(_fd,0) < 0){
            printf("Not found device in address 0x%x \n",addr);
            return 0;
        }
        else{
            printf("Found device in address 0x%x \n",addr);
            return 1 ;
        }
    }
}
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
bool useLcd = true;

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
    if(detectI2C(0x27)){
        pcf8574_address = 0x27;
    }else if(detectI2C(0x3F)){
        pcf8574_address = 0x3F;
    }else{
        printf("No correct I2C address found, \n"
        "Please use command 'i2cdetect -y 1' to check the I2C address! \n"
	);
	useLcd = false;
    }
    if(useLcd){
	    pcf8574Setup(BASE,pcf8574_address);//initialize PCF8574
	    for(int i=0;i<8;i++){
		pinMode(BASE+i,OUTPUT);     //set PCF8574 port to output mode
	    }
	    digitalWrite(LED,HIGH);     //turn on LCD backlight
	    digitalWrite(RW,LOW);       //allow writing to LCD
		lcdhd = lcdInit(2,16,4,RS,EN,D4,D5,D6,D7,0,0,0,0);// initialize LCD and return “handle” used to handle LCD
	    if(lcdhd == -1){
		printf("lcdInit failed !");
		return 1;
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
	    if(useLcd) lcdDisplay(lcdhd, 1);
	    cout << "Key : " << key << endl;
	    char message[2] = {key, '\0'};
	    char reply[REPLY_LENGHT];
	
            long statusCode;	    
	    string send = send_key(key, &statusCode);
	    cout << "Message : " << send << endl;
            /* printKey(lcdhd, key);
	    if(statusCode != 200){
		    cerr << "Erreur : code " << statusCode << endl;
	    }
	    if(useLcd){
		    
		    printStatusCode(lcdhd, statusCode);
	    }*/
	    if(statusCode != 200){
		    cerr << "Erreur : code " << statusCode << endl;
	    }
	    if(useLcd){
		printKey(lcdhd, key);
		if(statusCode != 200)
			printStatusCode(lcdhd, statusCode);
		else {
			if(callback_exists(key))
			   	executeCallback(key, send, lcdhd);
		}
	    }
	}
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
