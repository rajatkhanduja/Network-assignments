/* This file defines the functions for the FtpServer Class and its methods. */

#include <FtpServer.h>
#include <cstdio>
#include <errno.h>

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

/* Function to let the newly spawned FtpServer object (after the accept 
 * method) to serve the client.
 * This separation from the main class facilitates multithreading.
 */
void FtpServer::serve ()
{
  string msg, reply;
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
    
    std::cerr << "Replied\n";
    // Reply appropriately.
//    *dataSocket << reply;    
  
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
