#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <wiringPi.h>
#include <string.h>

#include "socket.h"

#define RED_LED		5
#define	GREEN_LED	12
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

using namespace std;

/**
 * Envoie un message et enregistre la reponse dans le dernier paramètre
 * @param message Message à envoyer
 * @param sock Socket
 * @param reply Variable dans laquelle sera stockée la reponse
 */
bool sendMessage(char *message, SOCKET sock, char* reply){
	if(send(sock, message, strlen(message), 0) == SOCKET_ERROR){
		return false;
	}
	string to_send = "raspberry_keypad\nenter-key\n"<< message
	cout << "Message envoyé : " << message << endl;
	if(recv(sock, reply, 10, 0) == SOCKET_ERROR){
		return false;
	}
	cout << "Reponse : " << reply << endl;
	cout << "hexval : ";
	for(const auto &item : string(reply)){
		cout << hex << int(item);
	}
	cout << endl;
	
	if(strstr(reply, "error") != NULL){
		cout << "error" << endl;
		return false;
	}
	cout << endl << endl;
	return true;
}

/**
 * faux main
 */
int test(int argc, char* argv[]){

	SOCKADDR_IN sin;
	
	sin.sin_addr.s_addr = inet_addr("192.168.0.11");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(3000);
	
	char *message, reply[255];
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR){
		cout << "Connexion réussie" << endl;
	} else {
		cout << "Connexion echouée" << endl;
	}
	
	message = "4";
		
	int send = sendMessage(message, sock, reply);
	delay(4000);
	send = sendMessage(message, sock, reply);
		
	close(sock);
	return 0;
}

SOCKADDR_IN getAddress(const char* host, int port){
	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = inet_addr(host);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	return sin;
}


