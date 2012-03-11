/* File containing the definition of FtpClient Class and its functions. */

#include <FtpClient.h>
#include <cstdlib>

bool FtpClient::connectToHost (const string& host, const int& port)
{
  this->host = host;
  return (commandPort.connect (host, port));
}

TcpSocket::Errors FtpClient::getErrorVal () const
{
  return commandPort.getErrorVal();
}

/* TODO : right now, only message/command is being sent,
 * Need to receive appropriate responses and get data.
 */

list<string> FtpClient::dir (const string& dir)
{
  command += (char) Ftp::Dir;
  command += dir;
  commandPort << command;

  string reply, tmp;
  commandPort >> reply;
  std::cerr << (int) reply[0];

  // Listen for data port number
  commandPort >> reply;
  if ( reply[0] == Ftp::PortVal )
  {
    reply.erase (0,1);
    tmp.clear();
    tmp += (char) Ftp::Accept;
    commandPort << tmp; 

    dataPort.connect (host, strtol (reply.c_str(), 0, 0));

    dataPort >> reply;
    
    std::cerr << reply;
    
    list<string> response;
    return response;
  }
}

bool FtpClient::getFile (const string& file)
{
  command += file;
  command.insert (0, 1, (char) Ftp::Get);
  commandPort << command;
}

bool FtpClient::getFiles (const list<string>& files)
{
  /* MGet is followed by a list of names, separated by
   * space.
   */
 
  list<string>::const_iterator itr, itr_end;
  string command;
  command += (char) Ftp::MGet;
  
  for ( itr = files.begin(), itr_end = files.end(); itr != itr_end; itr++)
  {
    command += " ";
    command += *(itr);    
  }
  
  commandPort << command;
}

bool FtpClient::terminate ()
{
  string command;
  command += (char) Ftp::Terminate;

  commandPort << command;
  
  commandPort >> command;
  
  return (command[0] == 1);
}
