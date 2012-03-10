/* File containing the declaration of the FtpClient Class and its methods. */
#ifndef FTP_CLIENT_INCLUDED
#define FTP_CLIENT_INCLUDED

#include <TcpSocket.h>

class FtpClient
{
  public:
    
    static const int defaultPort = 8765;

    // Connection-related methods.
    bool connectToHost (const string& host, const int& port = defaultPort);

    TcpSocket::Errors getErrorVal () const;

  private:
    string command;
    TcpSocket dataPort, commandPort;

};

#endif // End of file
