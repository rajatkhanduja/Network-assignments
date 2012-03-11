/* File contains the declaration for FtpServer class and its methods. */
#ifndef FTP_SERVER_INCLUDED
#define FTP_SERVER_INCLUDED

#include <TcpSocket.h>

class FtpServer
{
  public:
    static const int defaultBackLog = 20;
    
    // Constructor
    FtpServer (int port = 0, const int& queueLength = defaultBackLog); // Sets up a server lisening to port with queue length
    FtpServer (int socketFd, const FtpServer& server);

    
    FtpServer * accept ();

  private:
    TcpSocket *listenSocket;
    TcpSocket *dataSocket;
};

#endif    // End of file
