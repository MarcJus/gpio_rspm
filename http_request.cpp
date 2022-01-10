#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstring>
#include <pcf8574.h>
#include <lcd.h>
#include <sstream>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string send_key(char key, long *statusCode){
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  char url[100] = "http://192.168.0.11:3000/key/";
  char key_char[2] = {key, '\0'};
  strcat(url, key_char);
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, statusCode);
  } else {
	  return "";
  }
  return readBuffer;
}

void printKey(int lcdhd, char key){
	lcdClear(lcdhd);
	lcdPosition(lcdhd, 0, 0);
	lcdPrintf(lcdhd, "Key : %c",key);	
}

void printStatusCode(int lcdhd, long statusCode){
	lcdPosition(lcdhd, 0, 1);
	std::stringstream message;
	char *code;
	if(statusCode == 200)
		//strcat(message, "Succes");
		message << "Succes";
	else{
		/*if(asprintf(&code, "%d", statusCode) != -1)
			strcat(strcpy(message, "Erreur ! Code "), code);*/
		message << "Erreur! Code " << statusCode;
	}	
       lcdPrintf(lcdhd, message.str().c_str());
}
