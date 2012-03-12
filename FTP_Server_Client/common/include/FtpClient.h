/* File containing the declaration of the FtpClient Class and its methods. */
#ifndef FTP_CLIENT_INCLUDED
#define FTP_CLIENT_INCLUDED

#include <TcpSocket.h>
#include <Ftp.h>
#include <string>
#include <list>

using std::list;
using std::string;

class FtpClient
{
  public:

    // Connection-related methods.
    bool connectToHost (const string& host, const int& port);

    TcpSocket::Errors getErrorVal () const;

    /* FTP commands */
    // List directory
    string listDir (const string& dir = string(), const bool& recursive = false);

    // List local directory
    string listLocalDir (const string& dir = string(), const bool& recursive = false);

    // Fetch file(s)
    bool getFile  (const string& file);
    bool getFiles (const list<string>& files = list<string>());

    // Put file(s)
    bool putFile  (const string& file);
    bool putFiles (const list<string>& files = list<string>());

    // Terminate connection
    bool terminate ();

  private:
    string command;
    string localDir;
    string remoteDir;
    string host;
    TcpSocket dataPort, commandPort;

    string * getData (Ftp::CommandCodes code, const string& arg);
};

#endif // End of file
