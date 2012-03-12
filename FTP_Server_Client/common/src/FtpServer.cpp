/* This file defines the functions for the FtpServer Class and its methods. */

#include <FtpServer.h>
#include <cstdio>
#include <errno.h>
#include <sstream>

using std::stringstream;

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

void FtpServer::setupDataSocket ()
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

    // Send the port number for the dataSocket.
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


}


/* Function to let the newly spawned FtpServer object (after the accept 
 * method) to serve the client.
 * This separation from the main class facilitates multithreading.
 */
void FtpServer::serve ()
{
  string msg, reply, tmp;
  int command;  
    
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
      // Send a Reject.
      std::cerr << "Incorrect command\n";
      reply = (char) Ftp::InvalidCommand;
      *listenSocket << reply;
    }
    
    if ( command == Ftp::Terminate)
    {
      break;
    }

    // Create the necessary data socket.
    dataSocket = new TcpSocket();
    
    if( !dataSocket )
    {
      throw "Couldn't open dataSocket\n";
    }
    dataSocket->bind();

    std::cerr << "Opened dataSocket\n" << std::endl;

    // Send the port number for the dataSocket.
    stringstream portString;
    portString << dataSocket->port();
    portString >> tmp;
    std::cerr << dataSocket->port() << " " << tmp << std::endl;
    reply.clear();
    reply += (char) Ftp::PortVal;
    reply += tmp;
    *listenSocket << reply;

    // Wait for confirmation (Accept)
    *listenSocket >> tmp;

    if ( (int) tmp[0] != Ftp::Accept)
    {
      throw "Unexpected Response received.\n";
    }

    /* Call appropriate functions to handle the command */
    reply = "ABC DEF";

    std::cerr << "Waiting for request on data socket..." << std::endl;

    // Reply appropriately.
    dataSocket->listen(Ftp::defaultBackLog);
    int newSocket = dataSocket->accept();
    TcpSocket *attachedDataSocket = new TcpSocket (newSocket, *dataSocket);
    
    *attachedDataSocket << reply;

    std::cerr << "Sent Data. Closing dataSocket" << std::endl;

    attachedDataSocket->close();
    dataSocket->close();


    if (command == Ftp::Terminate)
    {
      return;
    }
    
    msg.clear ();
    reply.clear ();
  }
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
