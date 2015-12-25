/*
 * The TCPStream class provides methods to send and receive data over a TCP/IP connection.
 * contains a connected socket descriptor and information about the peer- either cliet or server-
 * in the for of the IP address and TCP port.
 *
 * TCPStream includes simple get methods that return address and port, but not the socket 
 * descriptor whcih is kept private
 */



#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include "keys.h"

using namespace std;
 
class TCPStream {
  int     m_sd;
  string  m_peerIP;
  int     m_peerPort;
  
  public:
    /*
     * grants friend privileges to the TCPConnector and TCPAcceptor classes so
     * they can access the TCPStream constructors to supply connected socket descriptors.
     */ 
    friend class TCPAcceptor;  
    friend class TCPConnector;
    ~TCPStream();
    
    ssize_t send(const char* buffer, size_t len);
    int sendLS();
    ssize_t receive(char* buffer, size_t len);
    string getPeerIP();
    int getPeerPort();
    char* getCMD(char* line, int index);
    int checkCommand(string& key, keys* permission);
    int splitString(string& stringToSplit, string& firstHalf, string& secondHalf);
  
  private:
    TCPStream(int sd, struct sockaddr_in* address);
    TCPStream();
    TCPStream(const TCPStream& stream);
};
