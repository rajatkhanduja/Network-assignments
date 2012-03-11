/* This file defines the functions for the FtpServer Class and its methods. */

#include <FtpServer.h>

FtpServer::FtpServer (int port, const int& queueLength)
{
  listenSocket = new TcpSocket();
  listenSocket->bindAndListen (port, queueLength);
  dataSocket = NULL;
}

FtpServer::FtpServer (int socketFD, const FtpServer& server)
{
  listenSocket = new TcpSocket (socketFD, *(server.listenSocket));
  dataSocket = NULL;
}

FtpServer * FtpServer::accept ()
{
  int newSocket;
  if ((newSocket = listenSocket->accept ()) != -1)
  {
    return (new FtpServer (newSocket, *this));
  }
}
