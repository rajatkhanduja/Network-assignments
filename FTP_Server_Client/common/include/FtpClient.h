/* File containing the declaration of the FtpClient Class and its methods. */
#ifndef FTP_CLIENT_INCLUDED
#define FTP_CLIENT_INCLUDED


class FtpClient 
{
  public:
    static const ServerPort = 8765;
    
    FtpClient ();
    int connectToHost (const string &host, int port = ServerPort);

  private:
    struct FtpPacket
    {
      int code;
      string arg;

    };
};

#endif // End of file
