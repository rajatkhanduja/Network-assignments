/* Program for the client of RTP
 *
 * Author : Rajat Khanduja
 * Date : 13/1/2012
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <common.h>
#include <assert.h>

int main (int argc, char *argv[])
{
  int socket_fd, n;
  socklen_t len;
  struct sockaddr_in server_addr;
  struct rtp_packet_t client_packet;

  if (argc != 3)
  {
    printf ("Usage: %s ipaddress filepath\n", argv[0]);
    return 1;
  }

  // Path size should be PATH_LEN-1
  if (strlen(argv[2]) >= PATH_LEN)
  {
    printf ("Filename is too long");
    return 2;
  }
  
  // Create socket file descriptor
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

  // Set server_addr.sin_zero[8] to zero (and set other values)
  memset (&server_addr, 0, sizeof (server_addr));
  server_addr.sin_family = AF_INET;     // For IPv4 connections
  inet_pton (AF_INET, argv[1], &server_addr.sin_addr);  // Set the server address.
  server_addr.sin_port = htons (SERVER_PORT);

  len = sizeof (server_addr);

  struct rtp_packet_t packet, tmp_packet;

  // Set values of packet
  packet.connection_id = -1;
  packet.status = 0;
  packet.type = (int16_t) REQUEST;
  strcpy (packet.buffer, argv[2]);
  
  // Send REQUEST
  convert_packet (&packet, HTON);
  fprintf (stderr, "Trying %s\n", argv[1]);
  sendto (socket_fd, &packet, sizeof (packet), 0, (struct sockaddr *)&server_addr, len);

  // Wait for REQUEST_ACK
  while (1)
  {
    while ( recv (socket_fd, &packet, sizeof (packet), 0) == -1 )
      /* Do nothing */ ;

    convert_packet (&packet, NTOH);
    
    if (packet.connection_id != -1 && packet.type == REQUEST_ACK )
    {
      // REQUEST_ACK received. Proceed
      fprintf (stderr, "Server is querying the timestamp of %s\n", argv[2]);
      break;
    }
  }

  while (1)
  {
    /* Wait for DONE */
    while ( recv (socket_fd, &packet, sizeof (packet), 0) == -1 )
      /* Do nothing */ ;
    
    convert_packet (&packet, NTOH);

    if (packet.connection_id != -1 && packet.type == DONE)
    { 
      // DONE received.
      if (0 == packet.status)
      {
        // Success
        fprintf (stderr, "Timestamp of %s is %s\n", argv[2], packet.buffer);
      }
      else
      {
        // Error. Print error code
        fprintf (stderr, "Server unable to determine timestamp. Error code : %d\n", packet.status);
      }

      /* Return DONE_ACK */
      packet.type = DONE_ACK;
      convert_packet (&packet, HTON);     // Convert to network byte order
      sendto (socket_fd, &packet, sizeof (packet), 0, (struct sockaddr *)&server_addr, len);  
      break;
    }
  }

  return 0;
}
