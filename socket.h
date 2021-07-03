#ifndef SOCKET_H
#define SOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

bool sendMessage(char* message, SOCKET sock, char* reply);
SOCKADDR_IN getAddress(const char* host, int port);

#endif
