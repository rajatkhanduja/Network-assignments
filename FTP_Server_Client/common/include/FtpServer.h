/* File contains the declaration for FtpServer class and its methods. */
#ifndef FTP_SERVER_INCLUDED
#define FTP_SERVER_INCLUDED

#include <TcpSocket.h>
#include <Ftp.h>

class FtpServer
{
  public:
    // Constructor
    
    // Sets up a server lisening to port with queue length
    FtpServer (int port = 0, const int& queueLength = Ftp::defaultBackLog); 
    FtpServer (int socketFd, const FtpServer& server);

    FtpServer * accept ();
    
    void serve ();

    int getCommandErrorVal() const;
    int getDataErrorVal() const;

  private:
    TcpSocket *listenSocket;
    TcpSocket *dataSocket;
};

#endif    // End of file
