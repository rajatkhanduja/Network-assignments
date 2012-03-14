/* The server program for FTP-like protocol
 * 
 * Author : Rajat Khanduja
 * Date : 10/3/2012
 *
 * Argumenets :-
 *  -p PORT   :- sets the port to be used as PORT. (defaults to 8765)
 *  -l LENGTH :- sets the queue length to LENGTH. (defaults to 20).
 *  -v        :- Verbose
 */

#include <iostream>
#include <FtpServer.h>
#include <cstdlib>
#include <errno.h>
#include <getopt.h>
#include <common.h>

using std::cerr;

void usage (char *progName)
{
  cerr << "Usage:-\n" << progName <<" [-p PORT] [-v] [-l LENGTH]\n";
  return;
}

int main (int argc, char *argv[])
{
  
  /* Variables set by the passed arguments */
  char verbose  = 0;
  int port      = Ftp::defaultPort;
  int backlog   = Ftp::defaultBackLog;
  /*  --- */

  /* Parse the arguments */
  int opt;
  
  while ((opt = getopt(argc, argv, "p:vl:")) != -1)
  {
    switch (opt)
    {
      case 'p' :
              errno = 0;
              port = strtol (optarg, NULL, 0);
              if (errno )
              {
                ARG_ERROR_MSG("Port unrecognized.\n");
                exit (PortUnrecognized);
              }
              break;

      case 'v' : verbose = 1; break;

      case 'l' : 
              errno = 0;
              backlog = strtol (optarg, NULL, 0);
              if (errno )
              {
                ARG_ERROR_MSG("Invalid value for -l.\n");
                exit (InvalidQueueLength);
              }
              break;

      default  :
              usage (argv[0]); 
              exit (UnrecognizedArg);    
    }
  }
  /* --- */
 
  FtpServer ftpServer(port, backlog);
  std::cerr << "Cleared this\n" << std::endl;
  FtpServer *newConn;
  
  while (newConn = ftpServer.accept())
  {
    std::cerr << "New Connection\n";   
    newConn->serve ();
    std::cerr << "End of transmission\n";
    delete newConn;
    newConn = NULL;
  }
  
  return 0;
}
