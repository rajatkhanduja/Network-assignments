/* File containing the definition of FtpClient Class and its functions. */

#include <FtpClient.h>

bool FtpClient::connectToHost (const string& host, const int& port)
{
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

  string reply;
  commandPort >> reply;
  std::cerr << (int) reply[0];

  std::cerr << "Done with the reply\n";
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
