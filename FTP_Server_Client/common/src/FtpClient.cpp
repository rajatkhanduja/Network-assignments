/* File containing the definition of FtpClient Class and its functions. */

#include <FtpClient.h>
#include <FtpCommands.h>
#include <cstdlib>
#include <sstream>

using std::istringstream;

const char * FtpClient :: defaultDir = "/tmp/client/";

FtpClient::FtpClient ()
{
  chdir (defaultDir);
  localDir  = string (defaultDir);
  remoteDir = string ("/tmp/");
  dataPortConnected = false;
}

string FtpClient::pwd () const 
{
  return remoteDir;
}

string FtpClient::lpwd () const
{
  return localDir;
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
  
  std::cerr << "Received : " << *finalReply << std::endl;
   
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
  

  // Listen for data port number.
  commandPort >> reply;

  if ( reply[0] != Ftp::PortVal )
  {
    reply = (char) Ftp::InvalidCommand;
    commandPort << reply;
    std::cerr << Ftp::response (reply[0]);
    throw ("Port value for data not received.\n");
  }

  reply.erase (0,1);
  tmp.clear();
  tmp += (char) Ftp::Accept;

  if ( !dataPortConnected )
  {
    dataPort.connect (host, strtol (reply.c_str(), 0, 0));
    dataPortConnected = true;
  }    
  
  std::cerr << "Connected to data port "<< reply << "\n";
  
  if ( code == Ftp::Put )
    return NULL;

  return getData();
    
}

string FtpClient::listDir (const string& dir, const bool& recursive)
{
  string * reply = getData (Ftp::Dir, dir);
  
//  closeDataPort();
  return *reply;
}

string FtpClient::listLocalDir (const string& dir, const bool& recursive)
{
  string * reply = (::dir (dir));

//  closeDataPort ();
  return *reply;
}

// TODO : Initially set the remote directory variable. 
bool FtpClient::changeDir (const string& dir)
{
  string * reply = getData (Ftp::ChDir, dir);

  if ( reply->compare ("Failed"))
  {
    remoteDir = *reply;
    return true;
  }
  else
  {
    return false;
  }
}


bool FtpClient::changeLocalDir (const string& dir)
{
  if (chdir (dir.c_str()) == 0)
  {
    localDir = dir;
    return true;
  }
  else
  {
    return false;
  }
}

bool FtpClient::getFiles (string& files)
{
  /* MGet is followed by a list of names, separated by
   * space.
   */

  int n = replaceSpaces (files) ;
  string * filename, *data;
  filename = getData (Ftp::Get, files);
  
  std::cerr << filename->length() << " " << (int) (*filename)[0] << std::endl;

  while ( (*filename)[0] != Ftp::Done && (*filename)[0] != Ftp::InvalidArg)
  {
    if ( !filename->compare ("") )
      break;
    data = getData ();
    std::cerr << "Filename : " << *filename << " :-\n";
    std::cerr << *data ;
    putFileStream (*filename, *data);
    filename = getData();
    n--;
  }
 
  std::cerr << (int) (*filename)[0] << std::endl;
  std::cerr << ((*filename)[0] == Ftp::InvalidArg) << std::endl;
  std::cerr << "Files received\n";

  if ( ((*filename)[0] == (int) Ftp::InvalidArg ) )
  {
    data = getData ();
    std::cerr << "Error in files:\n" << *data;
  }

  std::cerr << "Done\n";

  closeDataPort ();
  return (n == 0);
}

bool FtpClient::putFiles (string &files)
{
  int n = replaceSpaces (files);
  n = (n > 0) ? n : 1;
  string data;
  ifstream * fileStream;

  // Setup the dataSocket for transmission.


  while ( n-- )
  {
    istringstream tmpStream(files);

    while (getline (tmpStream, files, '\n') )
    {
      fileStream = getFileStream (files);
      
      if( *fileStream )
      {
        *fileStream >> data;
        dataPort <<  data;
      }

    }
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

bool FtpClient::closeDataPort ()
{
  return ( dataPort.close());
}
