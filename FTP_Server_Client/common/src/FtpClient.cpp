/* File containing the definition of FtpClient Class and its functions. */

#include <FtpClient.h>
#include <FtpCommands.h>
#include <cstdlib>

const char * FtpClient :: defaultDir = "/tmp/client/";

FtpClient::FtpClient ()
{
  chdir (defaultDir);
}

bool FtpClient::connectToHost (const string& host, const int& port)
{
  this->host = host;
  return (commandPort.connect (host, port));
}

TcpSocket::Errors FtpClient::getErrorVal () const
{
  return commandPort.getErrorVal();
}


/* Different from getData (...) as it does not 'connect' again. It assumes
 * the dataport is already connected and waits for data. 
 */
string * FtpClient::getData ()
{
  string *finalReply = new string();
  dataPort >> *finalReply;
  
  std::cerr << "Received"<< std::endl;
   
  return finalReply;
}

string * FtpClient::getData (Ftp::CommandCodes code, const string& arg)
{
  string command, reply, tmp;

  // Send command
  command += (char) code;
  command += arg;
  commandPort << command;

  // Wait for the reply.
  commandPort >> reply;

  if ( reply[0] != Ftp::Accept)
  {
    std::cerr << Ftp::response (reply[0]);
    return (new string());
  }
  

  // Listen for data port number or Invalid argument.
  commandPort >> reply;

  if ( reply[0] != Ftp::PortVal )
  {
    std::cerr << Ftp::response (reply[0]);
    return (new string());
  }

  reply.erase (0,1);
  tmp.clear();
  tmp += (char) Ftp::Accept;
  commandPort << tmp; 

  dataPort.connect (host, strtol (reply.c_str(), 0, 0));

  return getData();
    
}

string FtpClient::listDir (const string& dir, const bool& recursive)
{
  string * reply = getData (Ftp::Dir, dir);
  
  return *reply;
}

string FtpClient::listLocalDir (const string& dir, const bool& recursive)
{
  string * reply = (::dir (dir));

  return *reply;
}

bool FtpClient::getFiles (string& files)
{
  /* MGet is followed by a list of names, separated by
   * space.
   */

  int n =  1 + replaceSpaces (files) ;
  std::cerr << n << std::endl;
  string * filename, *data;
  filename = getData ( (n > 1) ? Ftp::Get : Ftp::MGet, files);

  while ( (*filename)[0] != Ftp::Done)
  {
    if ( (filename->compare ("")) )
    {
      data = getData ();
    }

    std::cerr << "Filename : " << *filename << " :-\n";
    std::cerr << *data ;
    putFileStream (*filename, *data);
    if ( n > 0 )
      filename = getData();
 }
 
  return true;
}

bool FtpClient::terminate ()
{
  string command;
  command += (char) Ftp::Terminate;

  commandPort << command;
  
  commandPort >> command;
  
  return (command[0] == 1);
}
