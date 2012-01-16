/* This file contains the declarations of the functions to be used by the server
 * program. This file includes 'common.h', which contains the structures and
 * constants to be used by both the server and the client
 *
 * Author : Rajat Khanduja
 * Date : 13/1/2012
 */

#ifndef SERVER_FUNCTIONS_H_INCLUDED
#define SERVER_FUNCTIONS_H_INCLUDED

#include <time.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <common.h>
#include <threads_list.h>

#define MAX_THREADS 200
#define MAX_CONN_ID 10
#define TIMESTAMP_LEN 30
struct server_thread_info
{
  struct rtp_packet_t packet;
  struct sockaddr_in client_addr;
  struct thread_list_elem list_elem;
};

time_t get_timestamp (char *filename, char *timestamp);

int sendPacket (struct rtp_packet_t *packet, sem_t *socket_sem, 
                struct sockaddr_in *client_addr, int socket_fd);
#endif  // End of file
