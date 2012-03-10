/* The server program for FTP-like protocol
 * 
 * Author : Rajat Khanduja
 * Date : 10/3/2012
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <common.h>
#include <errno.h>

#define MAX_PENDING 10

enum errors {
  SOCKET_FD_ERR = 1,
  SOCKET_BIND_ERR,
  LISTENING_ERR
};

int main (int argc, char *argv[])
{
  int n;
  socklen_t len;
  struct sockaddr_in server_addr, client_addr;
  int socket_fd;
  int port = SERVER_PORT;

  /* Initial setup */
  
  // Create a socket file descriptor
  socket_fd = socket (AF_INET, SOCK_STREAM, 0);
  
  if ( !socket_fd )
  {
    /* Report error and exit */
    perror ("socket_fd");
    exit (SOCKET_FD_ERR);
  }

  // Set the address location to zero
  memset ((void *) &server_addr, 0, sizeof (server_addr));  // To set server_addr.sin_zero[8] to zero
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
  /* --- */

  /* Parse input arguments */
  // TODO
  /* --- */

  // Set the port
  server_addr.sin_port = htons (port);

  /* Bind to the port */
  if ( bind (socket_fd, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0 )
  {
    /* Report error and exit */
    perror ("bind()");
    exit (SOCKET_BIND_ERR);
  }

  /* listen for incoming request */
  if ( listen (socket_fd, MAX_PENDING)< 0)
  {
    /* Report error and exit */
    perror ("listen()");
    exit (LISTENING_ERR);
  }

  while (1)
  {
    len = sizeof (client_addr);

    if ( accept (socket_fd, (struct sockaddr *) &client_addr, &len) < 0)
    {
      perror ("accept()");
    }

    printf ("Accepted connection from %s\n", inet_ntoa(client_addr.sin_addr));
  }
}
