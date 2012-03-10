/* This file declares the Socket class and its methods */

#ifndef TCP_SOCKET_INCLUDED
#define TCP_SOCKET_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>


#include <string>

using std::string;
using std::istream;
using std::ostream;

class TcpSocket
{
  public:
    static const int defaultPort = 8765;
    static const int defaultBackLog = 20;

    // Enumerations
    enum Errrors
    {
      None = 0,
      SocketFdErr,
      SocketBindErr,
      ListeningErr,
      ConnectErr,
      UnknownError
    } errorVal;

    
    // Constructor
    TcpSocket ();

    // Methods to establish a connection
    bool connectToHost (const string& host, const int& port = defaultPort);
    int accept ();
    bool bind ();
    bool listen (const int& backLog = defaultBackLog);
    bool bindAndListen (const int& backLog = defaultBackLog);

    // Sending and receiving data
    void operator >> (string& line);
    void operator << (string& msg );
    
  private:
    struct sockaddr_in hostAddr;
    socklen_t len;
    int socketFD;
    istream *sockStreamIn;
    ostream *sockStreamOut;
};

#endif 
