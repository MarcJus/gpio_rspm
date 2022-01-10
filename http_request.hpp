#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include <iostream>

std::string send_key(char key, long *statusCode);
void printKey(int lcdhd, char key);
void printStatusCode(int lcdhd, long statusCode);

#endif //HTTP_REQUEST
