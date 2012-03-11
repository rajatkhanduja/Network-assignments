/* File containing definition of the TcpSocket class and its methods */

#include <TcpSocket.h>
#include <cstring>
#include <ext/stdio_filebuf.h>
#include <cassert>
#include <netinet/in.h>
#include <arpa/inet.h>

using __gnu_cxx::stdio_filebuf;

inline stdio_filebuf<char> * fileBufFromFD (int fd, std::_Ios_Openmode mode)
{
  return (new stdio_filebuf<char> (fd, mode));
}

istream * createInStreamFromFD (int fd)
{
  stdio_filebuf<char> * fileBuf = fileBufFromFD (fd, std::ios::in);
  return (new istream (fileBuf)); 
}

ostream * createOutStreamFromFD (int fd)
{
  stdio_filebuf<char> * fileBuf = fileBufFromFD (fd, std::ios::out);
  return  (new ostream (fileBuf));
}


int TcpSocket::setupSocket (int socketFD)
{
  if (socketFD == 0)
  {
    // Create socket file descriptor
    socketFD = socket (AF_INET, SOCK_STREAM, 0);

    len = 0;
    errorVal = None;

    if (socketFD < 0)
    {
      errorVal = SocketFdErr;
      throw SocketFdErr;
    }
  }

  sockStreamIn  = createInStreamFromFD (socketFD);
  sockStreamOut = createOutStreamFromFD(socketFD);

  return socketFD;
}

TcpSocket::TcpSocket ()
{
  // Set server_addr.sin_zero[8] to zero.
  memset (&hostAddr, 0, sizeof (hostAddr));
  hostAddr.sin_family = AF_INET;   // For IPv4 connections.

  socketFD = setupSocket (); // Changes the value of socketFD and creates streams.
}

TcpSocket::TcpSocket (int socketFD, const TcpSocket& socket)
{
  memcpy (&(this->hostAddr), &(socket.hostAddr), socket.len);
  len = sizeof (this->hostAddr);

  assert (this->len == socket.len);

  setupSocket (socketFD);  
}


bool TcpSocket::connect (const string &host, const int& port)
{ 
  char isHostAddrSet = inet_pton (AF_INET, host.c_str(), &hostAddr.sin_addr);
 
  if (isHostAddrSet == 0)
  {
    errorVal = InvalidAddr;
    return false;
  }
              
  if (isHostAddrSet == -1)
  {
    errorVal = UnsupportedAddr;
    return false;
  }
  
  hostAddr.sin_port = htons (port);
    
  len = sizeof (hostAddr);    

  if  (::connect (socketFD, (struct sockaddr*) &hostAddr, len) != 0)
  {
    errorVal = ConnectErr;
    return false;
  }
  else
  {
    return true;
  }
}


/* Inline these functions */
inline bool TcpSocket::bind (const int& port)
{
  hostAddr.sin_port = htons(port);
  hostAddr.sin_addr.s_addr = htonl (INADDR_ANY);
  len = sizeof (hostAddr);
  return ( (:: bind (socketFD, (struct sockaddr*) &hostAddr, len) )== 0);
}

inline bool TcpSocket::listen (const int& backLog)
{
  return ( :: listen (socketFD, backLog) == 0 );
}
#include <stdio.h>
#include <errno.h>

bool TcpSocket::bindAndListen (const int& port, const int& backLog)
{
  
  fprintf (stderr, "%d\n", port);
  if ( ! this->bind(port) )
  {
    perror ("bind");
    errorVal = SocketBindErr;
    return false;
  }

  if ( ! this->listen (backLog) )
  {
    errorVal = ListeningErr;
    return false;
  }

  return true;
}

int TcpSocket::accept ()
{
  return ( :: accept (socketFD, (struct sockaddr*) &hostAddr, &len) );
}

bool TcpSocket::operator >> (string &line)
{
  std::getline (*sockStreamIn, line, (char) (EOF));

  return (line.length());
}

void TcpSocket::operator << (const string& msg)
{
  *sockStreamOut << msg; 
  *sockStreamOut << (char) (EOF);
  sockStreamOut->flush();
}

TcpSocket::Errors TcpSocket::getErrorVal () const
{
  return errorVal;
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

string TcpSocket::getError() const
{
  switch (errorVal)
  {
    case None           : return string("None");
    case SocketFdErr    : return string("Socket File-descriptor Error");
    case SocketBindErr  : return string("Socket Binding Error");
    case ListeningErr   : return string("Error listening");
    case InvalidAddr    : return string("Invalid host address");
    case UnsupportedAddr: return string("Unsupported host address");
    case ConnectErr     : return string("Connecting error");
    default             : return string("Unknown Error");
  }
}

