/* File contains the definition of functions to be used by the server
 * Author : Rajat Khanduja 
 * Date : 13/1/2012
 */ 

#include <server_functions.h>
#include <sys/stat.h>         // for stat()
#include <stdio.h>
#include <errno.h>

sem_t connection_id_sem;
unsigned int connection_id;     // Stores the last connection_id

time_t get_timestamp (char *filename, char *timestamp)
{
  // Get the stats.
  struct stat file_stats;
  int success;
  if ( (success = stat (filename, &file_stats)) < 0)
  {
//    strerror_r (errno, timestamp, PATH_LEN - 1);
//    fprintf (stderr, "stat : %s\n", timestamp);
    return (success);
  }
  
  time_t last_mod_time = file_stats.st_mtime;

  if (timestamp)
  {
    struct tm tm_result;

    /* Convert time to string */
    gmtime_r (&last_mod_time, &tm_result);
    strftime (timestamp, TIMESTAMP_LEN, "%a %b %e %H:%M:%S %Y", &tm_result);
  }
  
  // Return the 'last modified time'.
  return last_mod_time;
}


int sendPacket (struct rtp_packet_t *packet, sem_t *socket_sem, 
                struct sockaddr_in *client_addr, int socket_fd)
{
  /* Get lock for socket */
  sem_wait (socket_sem);
  
  socklen_t len = sizeof ( *client_addr );
  /* send the packet */
  sendto (socket_fd, packet, sizeof (*packet) , 0, (struct sockaddr *)client_addr, len); 
  
  /* Release lock */
  sem_post (socket_sem);
}

