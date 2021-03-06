/* File containing the definition of FtpClient Class and its functions. */

#include <FtpClient.h>
#include <FtpCommands.h>
#include <cstdlib>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>


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


/* Function to setup the data socket with the server. 
 * This function is required before sending any DATA. One may setup the 
 * socket once. Every call made to this function after that is 
 * redundant and it is taken care by the function that the dataSocket is not
 * re-established. 
 * 
 * The function sends the necessary initial code and argument and then sets up
 * the data socket. So any command and argument should be passed to this and 
 * this function will communicate with the server.
 */
void FtpClient::setupDataSocket (Ftp::CommandCodes code, const string& arg)
{
  string command, reply, tmp;

  // Send command
  command += (char) code;
  
  if ( code != Ftp::Put && code != Ftp::RPut)
    command += arg;
  
  commandPort << command;

  // Wait for the reply.
  commandPort >> reply;

  if ( reply[0] != Ftp::Accept)
  {
    std::cerr << Ftp::response (reply[0]);
    throw string("Unexpected response.");
  }

  // If the data port is not already connected, establish a connection.
  if ( !dataPortConnected )
  {
    // Listen for data port number.
    commandPort >> reply;

    if ( reply[0] != Ftp::PortVal )
    {
      reply = (char) Ftp::InvalidCommand;
      commandPort << reply;
      std::cerr << Ftp::response (reply[0]);
      throw string ("Port value for data not received.\n");
    }

    reply.erase (0,1);
    tmp.clear();
    tmp += (char) Ftp::Accept;

    dataPort.connect (host, strtol (reply.c_str(), 0, 0));
    dataPortConnected = true;
    
    std::cerr << "Connected to data port "<< reply << "\n";
  }    

  return ;
}

/* This function has to be called the first time some data has to be fetched
 * from the server. This sets up the datasocket and gets the data. Use this 
 * everytime when first establishing the connection with the server for any
 * command. Henceforth, use getData() as it returns the data that follows 
 * regardless of the command and the argument(s) that were passed. 
 */
string * FtpClient::getData (Ftp::CommandCodes code, const string& arg)
{
  setupDataSocket(code, arg);
  
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

bool FtpClient::getFiles (string& files, const bool& recursive)
{
  /* MGet is followed by a list of names, separated by
   * space.
   */

  replaceSpaces (files) ;
  setupDataSocket (recursive ? Ftp::RGet : Ftp::Get, files);
  
  string errFiles = recvFileData (&dataPort);  

  if ( errFiles.length())
  {
    std::cerr << "Files unsuccessfully fetched\n" << errFiles;
    return false;
  }
  else
    return true;
}


bool FtpClient::putFiles (string& files, const bool& recursive)
{
  int n = replaceSpaces (files);
  n = (n > 0) ? n : 1;
  string data;
  
  // Setup the dataSocket for transmission.
  setupDataSocket ( recursive ? Ftp::RPut : Ftp::Put, string());

  // Send data and get error files. 
  string errorFiles = sendFileData (files, recursive, dataPort);

  if ( errorFiles.length() )
  {
    // Report error to the client.
    std::cerr << "Files that couldn't be pushed : \n" << errorFiles;
    return false;
  }
  else
  {
    return true;
  }
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
