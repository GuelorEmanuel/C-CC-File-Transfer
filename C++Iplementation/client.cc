#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <locale>
#include "tcpconnector.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("usage: %s <port> <ip>\n", argv[0]);
    exit(1);
  }
  int len;
  int flag = 0;
  string message;
  char line[256];
  string input = "";
  string firstHalfOfInput = "";
  string secondHalfOfInput = "";
  TCPConnector* connector = new TCPConnector();
  TCPStream* stream = connector->connect(argv[2], atoi(argv[1]));
  
  do {
    //len = stream->receive(line, sizeof(line));
    //printf("client---: %d\n", len);
    printf( "\nPlease enter a valid command: ");
    getline(cin, input);
    if (input.compare("q") == 0 || input.compare("quit") == 0) {
      printf("Good bye\n");
      flag = -1;
      delete stream;
      exit(0);
    }
    
    message = input;
    if (stream && !message.empty()) {
      if (stream->splitString(input,firstHalfOfInput, secondHalfOfInput) != -1 ) { 
      stream->send(input.c_str(), input.size());
      printf("sent - %s\n", message.c_str());
      if ( (len = stream->receive(line, sizeof(line)) ) > 0 && (strcmp(line,"stop") != 0)) {     
        line[len] = '\0';
        printf("received - %s\n", line);
      }
      else if ( !(len > 0)) {
        printf("Something went wrong!\n");
      }
      }
      else {
        printf ("Bash: Command not found");
      }
  }
  }while(flag != -1);
}
