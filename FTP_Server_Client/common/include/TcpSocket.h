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

    // Enumerations
    enum Errors
    {
      None = 0,
      SocketFdErr,
      SocketBindErr,
      ListeningErr,
      InvalidAddr,
      UnsupportedAddr,
      ConnectErr,
      UnknownError
    };

    
    // Constructor
    TcpSocket ();
    TcpSocket (int socketFD, const TcpSocket& socket);

    // Methods to establish a connection
    bool connect (const string& host, const int& port);
    int accept ();
    bool bind (const int& port = 0);
    bool listen (const int& backLog);
    bool bindAndListen (const int& port, const int& backLog);
    bool close ();

    int port () const;
    
    // Sending and receiving data
    bool operator >> (string& line);
    void operator << (const string& msg );

    Errors getErrorVal () const;
    string getError () const;
    
  private:
    struct sockaddr_in hostAddr;
    socklen_t len;
    int socketFD;
    istream *sockStreamIn;
    ostream *sockStreamOut;
    Errors errorVal;

    int setupSocket (int socketFD = 0);
};

inline int TcpSocket::port () const
{
  return ntohs(hostAddr.sin_port);
}

inline bool TcpSocket::close()
{
  if (::close (socketFD) < 0)
  {
    return false;
  }
  else
  {
    socketFD = 0;
    return true;
  }
}

inline TcpSocket::Errors TcpSocket::getErrorVal () const
{
  return errorVal;
}

inline bool TcpSocket::listen (const int& backLog)
{
  return ( :: listen (socketFD, backLog) == 0 );
}

inline bool TcpSocket::bind (const int& port)
{
  hostAddr.sin_port = htons(port);
  hostAddr.sin_addr.s_addr = htonl (INADDR_ANY);
  len = sizeof (hostAddr);
  if ( (:: bind (socketFD, (struct sockaddr*) &hostAddr, len) )== 0)
  {
    if ( port == 0)
    {
      getsockname (socketFD, (struct sockaddr*) &hostAddr, &len);
    }
    return true;
  }
  else
  {
    return false;
  }
}

#endif 
