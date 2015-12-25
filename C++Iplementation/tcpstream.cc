#include <stdio.h>
#include <arpa/inet.h>
#include <dirent.h>
#include "tcpstream.h"
#include "keys.h"

/*
 * Constructor stores teh connected socket descriptor then convert the socket 
 * information structure fields to a peer IP address string and peer TCP port
 *
 */ 
TCPStream::TCPStream(int sd, struct sockaddr_in* address) : m_sd(sd) {
	char ip[50];
	inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);
	m_peerIP = ip;
	m_peerPort = ntohs(address->sin_port);
	

}

/*
 * Destructor for simply closing the connection
 */ 
TCPStream::~TCPStream() {
  close(m_sd);
}

/*
 * This method Simply wrap calls to write(), returns the number of bytes sent
 * @param  buffer, len
 * @return ssize_t: size of blocks that can be writen
 */
ssize_t TCPStream::send(const char* buffer, size_t len){
  return write(m_sd, buffer, len);
}

int TCPStream::sendLS() {
  DIR *dp;
  struct dirent *ep;
  char* line[256];
  char  newLine[2] = {'\n','\0'};
  int i = 0;
  int count = 0;

  dp = opendir("./");
  if ( dp != NULL) {
    while (ep = readdir(dp)) {
      line[i] = ep->d_name;
	//send(ep->d_name,strlen(ep->d_name));
	//printf("size: %d\n", strlen(ep->d_name));
	//printf("here - %s\n", ep->d_name);
	//send(newLine.c_str(),newLine.size());
	count += strlen(ep->d_name);
	count += strlen(newLine);
    }
    //send(end, strlen(end));
    (void) closedir(dp);
    return count;

    }
  else {
    perror("couldn't open the directory");
    count = -1;
    return count;
  }
}

/*
 * This method Simply wrap calls to read(), returns the number of bytes received
 * @param  buffer, len
 * @return ssize_t
 */
ssize_t TCPStream::receive(char* buffer, size_t len) {
  return read(m_sd, buffer, len);
}

/*
 * These  methods return the IP and TCP port information of the peer to which
 * the network application, client or server, are connected.
 */
string TCPStream::getPeerIP(){return m_peerIP;}
int TCPStream::getPeerPort(){return m_peerPort;}
char* TCPStream::getCMD(char* line, int index){
  char temp[index];
  int count = 0;  
  for ( int i = 0; i < index; i++) { 
    temp[i] = line[i];
    count++;
    printf("index[%i]: %c\n",i,line[i]);
  }
  printf("index[%i]: %c\n", 0, line[0]);
  printf("index[%i]: %c\n", 1, line[1]);
  printf("index[%i]: %c\n", 2, line[2]);
  temp[count+1] = '\0';
  return temp;
}

int TCPStream::checkCommand(string& userInput, keys* permission) {

 *permission =  {"ls","get","put","cd","mkdir"};

 if ( strcmp(userInput.c_str(), permission->ls.c_str()) == 0) {
   return sendLS();
 }
 else if ( strcmp(userInput.c_str(), permission->get.c_str()) == 0) {
   return 0;
 }
 else if ( strcmp(userInput.c_str(), permission->put.c_str()) == 0) {
   return 0;
 }
 else if ( strcmp(userInput.c_str(), permission->cd.c_str()) == 0) {
   return  -1;//chdir(line);;
 }
 else if ( strcmp(userInput.c_str(), permission->mkdir.c_str()) == 0) {
   return -1;//mkdir(line, 0777);
 }
 else {
   return -1;
 }
 return 0;

}


int TCPStream::splitString(string& stringToSplit, string& firstHalf, string& secondHalf) {
        int spaceCounter = 0;
        char space = ' ';

	for ( int i = 0; i < stringToSplit.length(); i++) {
          if (stringToSplit.c_str()[i] == space) {
            spaceCounter++;
	    firstHalf  = stringToSplit.substr(0, i);
	    secondHalf = stringToSplit.substr(i);
	    break;	  
          }
	  else {
	    spaceCounter++;
	  }
	}
	if ( spaceCounter == 1) {
	  return 0;
	}
	else if(spaceCounter == stringToSplit.length()) {
	  firstHalf = stringToSplit;
	  return 0;
	}
	else {
          return -1;
          printf("Bash: command not found\n");
	}
}
