/* File contains the declaration for FtpServer class and its methods. */
#ifndef FTP_SERVER_INCLUDED
#define FTP_SERVER_INCLUDED

#include <TcpSocket.h>
#include <Ftp.h>

class FtpServer
{
  public:
    static const char* defaultDir;
    /* Constructor */
    // Sets up a server lisening to port with queue length
    FtpServer (int port = 0, const int& queueLength = Ftp::defaultBackLog); 
    FtpServer (int socketFd, const FtpServer& server);

    // Destructor
//    ~FtpServer();

    FtpServer * accept ();
    
    void serve ();

    int getCommandErrorVal() const;
    int getDataErrorVal() const;

  private:
    TcpSocket *listenSocket;
    TcpSocket *dataSocket;
    TcpSocket *openSocket;

    // Private functions.
    void setupDataSocket ();
    void handleCommand (const int& command, const string& arg);
    bool getCommandHandler (const string& filename);
};


#endif    // End of file
