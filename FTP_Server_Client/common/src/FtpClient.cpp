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

bool isCorrectResponse (int received, int expected)
{
  if ( received == expected )
  {
    return true;
  }

  switch ( received )
  {
    case Ftp::PermReject :
            throw string ("Server refusing connection.\n");
            return false;

    case Ftp::TempReject:
            throw string ("Server temporarily refusion connection. Try later.\n");
            return false;

    case Ftp::InvalidCommand:
            throw string ("Invalid Command\n");
            return false;
    default :
            throw string ("Unexpected response\n");
            return false;
  }
}

/* TODO : right now, only message/command is being sent,
 * Need to receive appropriate responses and get data.
 */

string * FtpClient::getData (Ftp::CommandCodes code, const string& arg)
{
  string command, reply, tmp;
  string *finalReply;

  // Send command
  command += (char) code;
  command += arg;
  commandPort << command;

  // Wait for the reply.
  commandPort >> reply;

  if ( ! isCorrectResponse (reply[0], Ftp::Accept))
  {
    return (new string());
  }
  

  // Listen for data port number
  commandPort >> reply;

  if ( ! isCorrectResponse (reply[0], Ftp::PortVal) )
  {
    return (new string());
  }
  reply.erase (0,1);
  tmp.clear();
  tmp += (char) Ftp::Accept;
  commandPort << tmp; 

  dataPort.connect (host, strtol (reply.c_str(), 0, 0));

  finalReply = new string();
  dataPort >> *finalReply;
      
    
  std::cerr << "Received : "<< reply << std::endl;
   
  return finalReply;
  
}

string FtpClient::listDir (const string& dir, const bool& recursive)
{
  string * reply = getData (Ftp::Dir, dir);
  
  return *reply;
}

bool FtpClient::getFile (const string& file)
{
  command += file;
  command.insert (0, 1, (char) Ftp::Get);
  commandPort << command;

  return true;
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
