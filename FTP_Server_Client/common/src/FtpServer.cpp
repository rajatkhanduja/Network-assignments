/* This file defines the functions for the FtpServer Class and its methods. */

#include <FtpServer.h>
#include <cstdio>
#include <errno.h>
#include <sstream>
#include <FtpCommands.h>
#include <fstream>
#include <list>

using std::stringstream;
using std::istringstream;
using std::ifstream;
using std::list;

const char* FtpServer::defaultDir = "/tmp/";

FtpServer::FtpServer (int port, const int& queueLength)
{
  listenSocket = new TcpSocket();
  fprintf (stderr, "%d\n", port);
  if (! listenSocket->bindAndListen (port, queueLength))
  {
    perror ("bindAndListen");
    throw listenSocket->getError();
  }
  dataSocket = NULL;
}

FtpServer::FtpServer (int socketFD, const FtpServer& server)
{
  listenSocket = new TcpSocket (socketFD, *(server.listenSocket));
  dataSocket = NULL;
}

void FtpServer::handleCommand (const int& command, const string& arg)
{
  /* Set up the datasocket. This includes sending information
   * about the socket to the client and waiting for it to connect 
   */
  TcpSocket *openSocket = NULL;
  string *tmp = NULL;
  
  switch (command)
  {
    case Ftp::Dir : 
              tmp = dir(arg);
              openSocket = setupDataSocket();
              *openSocket << *tmp;
              break;

    case Ftp::ChDir:
              openSocket = setupDataSocket();
              if ( 0 == chdir (arg.c_str()))
              { 
                *openSocket << arg;
              }
              else
              {
                tmp = new string();
                tmp->clear();
                *tmp += "Failed :";
                *tmp += arg; 
                *tmp += "\n";
                *openSocket << *tmp;
                delete tmp;
              }
    
    case Ftp::Get :
              istringstream tmpStream(arg);
              string token;
              openSocket = setupDataSocket();
              std::cerr << "Beginning processing" << std::endl;
              
              string errFiles;
              while ( std::getline(tmpStream, token, '\n'))
              {
                std::cerr << "Getting file " << token << std::endl;
                std::cerr << (int) token[token.length() - 1];
                ifstream *fileStream = getFileStream (token);
                if ( fileStream)
                {
                  std::cerr << "File opened.\n";
                  *openSocket << token;
                  *openSocket << (*fileStream);
                  fileStream->close();
                }
                else
                {
                  errFiles += string(token); 
                  errFiles += "\n";
                  std::cerr << "Added " << token << " to errFiles" << std::endl;
                  std::cerr << errFiles << std::endl;
                }
               }

               std::cerr << errFiles.length(); 

               if ( errFiles.length () > 0 )
               {
                  tmp = new string();
                  *tmp += (char) Ftp::InvalidArg;
                  std::cerr << "Sending error message\n";
                  *openSocket << *tmp;
                  std::cerr << "Sent error message\n";
                  *openSocket << errFiles;
                  std::cerr << "Sent files\n";
               }

               errFiles.clear ();
               errFiles += (char) Ftp::Done;

               *openSocket << errFiles;

               break;
  }

  if ( openSocket )
  {
    std::cerr << "Sent Data. Closing dataSocket" << std::endl;
    openSocket->close();
    dataSocket->close();
  }

  return;
}


TcpSocket * FtpServer::setupDataSocket ()
{
    string portMsg, tmp;
    
    // Create the necessary data socket.
    dataSocket = new TcpSocket();
    
    if( !dataSocket )
    {
      throw "Couldn't open dataSocket\n";
    }
    dataSocket->bind();

    std::cerr << "Opened dataSocket\n" << std::endl;

    /* Send the port number for the dataSocket. */

    // Convert port number to string.
    stringstream portString;
    portString << dataSocket->port();
    portString >> tmp;

    std::cerr << dataSocket->port() << " " << tmp << std::endl;
    
    portMsg += (char) Ftp::PortVal;
    portMsg += tmp;
    
    *listenSocket << portMsg;

    // Wait for confirmation (Accept)
    *listenSocket >> tmp;

    if ( (int) tmp[0] != Ftp::Accept)
    {
      throw "Unexpected Response received.\n";
    }


    dataSocket->listen(Ftp::defaultBackLog);
    int newSocket = dataSocket->accept();
    
//    std::cerr << "Returning attached socket.\n" << std::endl;
    
    return (new TcpSocket (newSocket, *dataSocket));
}


/* Function to let the newly spawned FtpServer object (after the accept 
 * method) to serve the client.
 * This separation from the main class facilitates multithreading.
 */
void FtpServer::serve ()
{
  string msg, reply, tmp;
  int command;  
  
  // Set the current directory to defaultDir.
  chdir (defaultDir);

  std::cerr << "Serving\n";  
  while ( *listenSocket >> msg )
  {
    // Recieve the command.

    command = msg[0];
    msg.erase (0,1);

    std::cerr << "Received : " << msg;

    /* This part is heavily dependent on the assumption/understanding
     * that the first character contains the command (explained in 
     * "Ftp.h").
     */
    if ( Ftp::isValidCommand (command))
    {
      // Correct command. Send 'accept'.
      std::cerr << "Correct command " << command << std::endl;
      reply = (char) Ftp::Accept;
      *listenSocket << reply;
    }
    else
    {
      // Send a Reject and listen for another command.
      std::cerr << "Incorrect command\n";
      reply = (char) Ftp::InvalidCommand;
      *listenSocket << reply;
      continue;
    }
    
    if ( command == Ftp::Terminate)
    {
      break;
    }

    /* Call appropriate functions to handle the command */
    handleCommand (command, msg);


    msg.clear ();
    reply.clear ();
  }

//  listenSocket->close();
  std::cerr << "Time to close this connection\n";
}

FtpServer * FtpServer::accept ()
{
  int newSocket;
  if ((newSocket = listenSocket->accept ()) != -1)
  {
    return (new FtpServer (newSocket, *this));
  }
  else
  {
    return NULL;
  }
}

int FtpServer::getDataErrorVal () const
{
  if ( !dataSocket ) 
    return dataSocket->getErrorVal();
  else
    return -1;
}

int FtpServer::getCommandErrorVal () const
{
  return listenSocket->getErrorVal();
}
