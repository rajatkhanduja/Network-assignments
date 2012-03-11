/* File containing definition of the TcpSocket class and its methods */

#include <TcpSocket.h>
#include <cstring>
#include <ext/stdio_filebuf.h>
#include <cassert>

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
  hostAddr.sin_port = htons (port);
  
  len = sizeof (hostAddr);    

  return (::connect (socketFD, (struct sockaddr*) &hostAddr, len) == 0);
}


/* Inline these functions */
inline bool TcpSocket::bind (const int& port)
{
  hostAddr.sin_port = htons(port);
  return ( :: bind (socketFD, (struct sockaddr*) &hostAddr, len) == 0);
}

inline bool TcpSocket::listen (const int& backLog)
{
  return ( :: listen (socketFD, backLog) == 0 );
}


bool TcpSocket::bindAndListen (const int& backLog, const int& port)
{
  
  if ( ! this->bind(port) )
  {
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

void TcpSocket::operator >> (string &line)
{
  std::getline (*sockStreamIn, line, (char) EOF);
}

void test()
{
  TcpSocket socket;
  string t;
  socket >> t; 
  socket << t;
}

void TcpSocket::operator << (string& msg)
{
  *sockStreamOut << msg; 
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
