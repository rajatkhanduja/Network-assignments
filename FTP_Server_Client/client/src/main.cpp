/* The client program for an FTP-like protocol.
 * Author : Rajat Khanduja
 * Date : 10/3/2012
 * 
 * Command line arguments :-
 *	-a HOST ADDRESS (mandatory)
 *	-p PORT	(defaults to 8765)
 *	-f FILE (read commands from FILE instead of stdin)
 *  -v
 */

#include <FtpClient.h>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <cstdlib>
#include <errno.h>
#include <common.h>

using std::cout;
using std::cerr;
using std::cin ;
using std::ifstream;

void usage (char *progName)
{
  cerr << "Usage:-\n" << progName <<" -a HOST_ADDRESS [-f FILE] [-p PORT] [-v]\n";
  return;
}


int main (int argc, char* argv[])
{
  /* Variables set by the passed arguments */
  string hostAddr;
  char verbose  = 0;
  int port = Ftp::defaultPort;
  istream *fpRead = &cin;
  /*  --- */

  /* Parse the arguments */
  int opt;
  
  while ((opt = getopt(argc, argv, "a:p:f:v")) != -1)
  {
    switch (opt)
    {
      case 'a' :
              // Set the server address.
              hostAddr = string (optarg);
              break;

      case 'p' :
              errno = 0;
              port = strtol (optarg, NULL, 0);
              std::cerr << "Port : "<< port <<  std::endl;
              if (errno )
              {
                ARG_ERROR_MSG("Port unrecognized.\n");
                exit (PortUnrecognized);
              }
              break;

      case 'f' :              
              // Set the file pointer.
              fpRead = new ifstream (optarg);

              if ( fpRead->fail() )
              {
                // Error opening file. Do not exit. Revert to stdin
                ARG_ERROR_MSG ("Unable to open file %s. Using stdin for input.\n")
                fpRead = &cin;
              }
              break;

      case 'v' : verbose = 1; break;

      default  :
              usage (argv[0]); 
              exit (UnrecognizedArg);    
    }
  }
  /* --- */

  // Ensure that host address is set.
  if (hostAddr.length() == 0)
  {
    cerr << argv[0] << " : Host address not provided\n";
    usage (argv[0]);
    exit (NoHost);
  }
  
  FtpClient client;

  if ( client.connectToHost(hostAddr, port) )
  {
    client.dir("/home/rajat/");
  }
  
  return 0;
}
