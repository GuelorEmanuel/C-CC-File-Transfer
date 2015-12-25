#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "tcpacceptor.h"

/*
 * The constructor sets the member variabls as shown here.
 * Setting m_lsd indicates that the listening socket has not been 
 * created.
 */
TCPAcceptor::TCPAcceptor(int port, const char* address): m_lsd(0), m_port(port), m_address(address), m_listening(false){}

/*
 * IF listeninig socket has been created then it is closed in
 * destructor
 */

TCPAcceptor::~TCPAcceptor() {
  if (m_lsd > 0 ) {
    close(m_lsd);
  }
}

/*
 * Checks to see if a listening socket already exists. If so,
 * the method just returns 0.
 */
int TCPAcceptor::start() {
  if (m_listening == true) {
    return 0;
  }

  m_lsd = socket(PF_INET, SOCK_STREAM,IPPROTO_TCP);
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = PF_INET;
  address.sin_port = htons(m_port);
  
  if (m_address.size() > 0 ) {
    inet_pton(PF_INET, m_address.c_str(), &(address.sin_addr)); // convert IPV4 and IPV6 addresses from text to binary form
  }
  else {
    address.sin_addr.s_addr = INADDR_ANY;
  }
  
  int optval = 1;
  setsockopt(m_lsd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
  int result = bind(m_lsd, (struct sockaddr*)&address, sizeof(address));
  
  if (result != 0) {
    perror("bind() failed");
    return result;					  
  }
  result = listen(m_lsd, 5);
  if (result !=0) {
    perror("listen() failed");
    return result;
  }
  m_listening = true;
  return result;
}

/*
 * returns NULL if the socket is not in a listening state. Otherwise a socaddr_in
 * structure is set to NULL and a pointer to it, cast as a sockaddr structure, is passed to ::accept().
 * The ::accept() call is qualified by the ::operator so the compiler does not confuse this function with
 * TCPAcceptor:accept(). The ::accept() blocks until a connection is received.
 *   
 */
TCPStream* TCPAcceptor::accept() {
  if (m_listening == false){
    return NULL;
  }
  
  struct sockaddr_in address;
  socklen_t len = sizeof(address);
  memset(&address, 0, sizeof(address));
  int sd = ::accept(m_lsd, (struct sockaddr*)&address, &len);
  
  if (sd < 0 ) {
    perror("accept() failed");
    return NULL;
  }
  return new TCPStream(sd, &address);
}

