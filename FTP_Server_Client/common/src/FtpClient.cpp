/* File containing the definition of FtpClient Class and its functions. */

#include <FtpClient.h>

bool FtpClient::connectToHost (const string& host, const int& port)
{
//  return commandPort.connect (host, port);
  if (commandPort.connect (host, port))
  {
    string test("test message\n");
    commandPort << test;
   
    // Wait for reply
    commandPort >> test;
    std::cerr << test;
    return true;
  }
  else return false;
}

TcpSocket::Errors FtpClient::getErrorVal () const
{
  return commandPort.getErrorVal();
}

/* TODO : right now, only message/command is being sent,
 * Need to receive appropriate responses and get data.
 */

bool FtpClient::dir (const string& dir)
{
  command += (char) Ftp::Dir;
  command += dir;
  commandPort << command;
}

bool FtpClient::getFile (const string& file)
{
  command += (char)Ftp::Get;
  command += file;
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
