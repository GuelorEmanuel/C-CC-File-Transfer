#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "tcpacceptor.h"

int main(int argc, char** argv) {
  keys* permissionKey = NULL;
  permissionKey = new keys;
  string requestTimeOut = "Something went wrong :(.";
  DIR *dp;
  struct dirent *ep;
  //char* line[256];
  int count = 0;
  char newLine[2] = {'\n','\0'};

  if (argc < 2 || argc > 4) {
    printf("usage: server <port> [<ip>]\n");
    exit(1);
  }
    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
    
    if (argc == 3) {
      acceptor = new TCPAcceptor(atoi(argv[1]), argv[2]);
    }
    else {
      acceptor = new TCPAcceptor(atoi(argv[1]));
    }
    if (acceptor->start() == 0) {
      while (1) {
        stream = acceptor->accept();
	if (stream != NULL) {
          ssize_t len;
	  char line[256];
	  int i = 0;
	  while ((len = stream->receive(line, sizeof(line))) > 0) {
	    line[len] = '\0';
	    printf("received - %s\n", line);
	    string rcvMessageToString(line);
	    string rcvMessageFirstHalf;
	    string rcvMessageSecondHalf;
	    stream->splitString(rcvMessageToString, rcvMessageFirstHalf, rcvMessageSecondHalf); 
	    if ( stream->checkCommand(rcvMessageFirstHalf, permissionKey) == 0){
	      //count = stream->sendLS();
	      
	    //}
	    }
	     /* else if (false) {
	        chdir(line);
	      }
	      else if (false) {
	        mkdir(line, 0777);
	      }*/
	    else {
	      stream->send(requestTimeOut.c_str(), requestTimeOut.size());
	    }
	  } 
	  delete stream;
	}
      } 
    }
    exit(0);
  }

