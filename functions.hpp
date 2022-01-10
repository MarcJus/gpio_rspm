#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <lcd.h>

typedef std::string (*Callback)(std::string);
void executeCallback(char key, std::string http_response, int lcdhd);
bool callback_exists(char key);

#endif // FUNCTIONS_H
