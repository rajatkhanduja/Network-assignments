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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <common.h>


enum ERRORS {
  UNRECOGNIZED_ARG = 1,
  MISSING_ARG,
  INVALID_ADDR,
  UNSUPPORTED_ADDR,
  NO_HOST,
  PORT_UNRECOGNIZED,
};


void usage (char *prog_name)
{
  fprintf (stderr, "Usage:-\n%s -a HOST_ADDRESS [-f FILE] [-p PORT]\n", prog_name);
  return;
}

int main (int argc, char *argv[])
{
  int socket_fd, n;
  socklen_t len;
  struct sockaddr_in server_addr;

  /* Variables set by the passed arguments */
  char host_addr_set = 0;
  char *host_addr;
  char verbose  = 0;
  int port = SERVER_PORT;
  FILE *fp_read = stdin;    // Readonly
  /*  --- */

  /* Initial setup */

  // Set server_addr.sin_zero[8] to zero (and set other values)
  memset (&server_addr, 0, sizeof (server_addr));
  server_addr.sin_family = AF_INET;     // For IPv4 connections
  
  // Create socket file descriptor
  socket_fd = socket (AF_INET, SOCK_STREAM, 0);
  
  /* --- */

  /* Parse the arguments */
  int opt;
  
  while ((opt = getopt(argc, argv, "a:p:f:v")) != -1)
  {
    switch (opt)
    {
      case 'a' :
              // Set the server address.
              host_addr_set = inet_pton (AF_INET, optarg, &server_addr.sin_addr);
              if (host_addr_set == 0)
              {
                ARG_ERROR_MSG ("%s is not a valid address\n");
                exit (INVALID_ADDR);
              }
              
              if (host_addr_set == -1)
              {
                ARG_ERROR_MSG ("Address : %s is not supported\n");
                exit (UNSUPPORTED_ADDR);
              }

              host_addr = optarg;

              break;

      case 'p' :
              errno = 0;
              port = strtol (optarg, NULL, 0);

              if (!errno )
              {
                ARG_ERROR_MSG("Port unrecognized.\n");
                exit (PORT_UNRECOGNIZED);
              }
              break;

      case 'f' :              
              // Set the file pointer.
              fp_read = fopen (optarg, "r");

              if ( !fp_read )
              {
                // Error opening file. Do not exit. Revert to stdin
                ARG_ERROR_MSG ("Unable to open file %s. Using stdin for input.\n")
              }
              break;

      case 'v' : verbose = 1; break;

      default  :
              usage (argv[0]); 
              exit (UNRECOGNIZED_ARG);    
    }
  }
  /* --- */

  // Ensure that host address is set.
  if (!host_addr_set)
  {
    fprintf (stderr, "%s : Host address not provided\n", argv[0]);
    usage (argv[0]);
    exit (NO_HOST);
  }
  
  /* More set up values, that required arguments */
  server_addr.sin_port = htons (port);

  len = sizeof (server_addr);

  char test_string[] = "Test packet.\n";
  test_string[strlen(test_string)] = EOF;
  /* --- */


 	/* Create a connection to the server */
  if ( connect (socket_fd, (struct sockaddr*) &server_addr, len) < 0 )
  {
    fprintf (stderr, "Could not connect to %s\n", host_addr);
  }

  send (socket_fd, test_string, strlen (test_string), 0);

  fprintf (stderr, "Sent\n");
  memset (test_string, 0, strlen(test_string));
  int t = recv (socket_fd, test_string, 12, 0);

  fprintf (stderr, "%s %d\n", test_string, t);
  return 0;
}
