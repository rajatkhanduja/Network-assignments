/* Program for the server of the RTP. 
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
#include <pthread.h>
#include <common.h>
#include <server_functions.h>
#include <semaphore.h>
#include <stdlib.h>

void new_thread_func (void *packet);

sem_t socket_sem;
int socket_fd;
unsigned int n_threads = 0;
pthread_mutex_t n_threads_mutex = PTHREAD_MUTEX_INITIALIZER;

int main (int argc, char *args[])
{
  int n;
  socklen_t len;
  struct sockaddr_in server_addr, client_addr;
  
  /* Create a list */
  LIST_INIT (&head);


  // Create a socket file descriptor
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

  // Set the address location to zero
  memset ((void *) &server_addr, 0, sizeof (server_addr));  // To set server_addr.sin_zero[8] to zero
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
  server_addr.sin_port = htons (SERVER_PORT);

  /* Bind to the port */
  bind (socket_fd, (struct sockaddr *) &server_addr, sizeof (server_addr));

  assert ( 0 == sem_init (&socket_sem, 0, 1));    // Assert that the socket semaphore has been initialized 

  fprintf (stderr, "Server running ...\n");

  struct rtp_packet_t packet;

  while (1)
  {
    len = sizeof (client_addr);

    while ( -1 == recvfrom (socket_fd, &packet, sizeof (packet) , 0, (struct sockaddr *)&client_addr, &len) )
      /* Wait until a packet without any error is received. */;


    convert_packet (&packet, NTOH);
    
    if ( -1 == packet.connection_id && REQUEST == packet.type )
    {
      fprintf (stderr, "Received a REQUEST.\n");
      fprintf (stderr, "Spawning new thread\n");
      

      struct server_thread_info *st = (struct server_thread_info *) malloc ( sizeof (struct server_thread_info) );
      st->packet = packet;
      st->client_addr = client_addr;
      st->list_elem.mutex = malloc ( sizeof (pthread_mutex_t) );
      *(st->list_elem.mutex) = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
      st->list_elem.condition_var = malloc ( sizeof (pthread_cond_t) );
      *(st->list_elem.condition_var) = (pthread_cond_t) PTHREAD_MUTEX_INITIALIZER;

      /* Lock the mutex, so that the thread waits for its release 
       * before exiting.
       */
      pthread_mutex_lock (st->list_elem.mutex);

      //Create a new thread
      if ( n_threads >= MAX_THREADS 
           ||
           0 != pthread_create ( &(st->list_elem.thread), NULL, (void *) &new_thread_func, (void *) st))
      {
        // Drop packet
        fprintf (stderr,"Unable to create a thread. Packet dropped.\n");
        continue;
      }

      pthread_detach (st->list_elem.thread);

      // Lock n_threads_mutex
      pthread_mutex_lock (&n_threads_mutex);

      // Increment the value;
      n_threads++;

      // Release lock.
      pthread_mutex_unlock (&n_threads_mutex);

      LIST_INSERT_HEAD (&head, &(st->list_elem), entries);
    }
    else if (DONE_ACK == packet.type && -1 != packet.connection_id )
    {
      /* TODO : Remove thread and set appropriate flags. */
      fprintf (stderr, "DONE_ACK received\n");
      
      // Find the correct thread
      struct thread_list_elem *t = thread_find (packet.connection_id, &head);
      if (t)
      {
        fprintf (stderr, "Signalling \n");
        // Signal thread
        pthread_cond_signal (t->condition_var);
        
        fprintf (stderr, "Releasing mutex \n");
        // Unlock mutex
        pthread_mutex_unlock (t->mutex);

/*        // Wait for the thread to end
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;

        if ( pthread_timedjoin_np (t->thread, NULL, &ts) )
        {
          fprintf (stderr, "Thread couldn't be joined.\n");
        }
        
        fprintf (stderr, "Thread joined\n"); */
       } 
      else
      {
        // No thread found corresponding to the connection id; drop packet
        fprintf (stderr, "Packet dropped. Connection_id unknown\n");
        continue;
      }

    }
    else
    {
      // Print packet type and drop it.
      fprintf (stderr, "A packet with type %d was dropped\n", (packet.type) );
    }

  }

  return 0;
}



void new_thread_func ( void *thread_info)
{
  /* Store value into local variables. */

  struct server_thread_info *st = ((struct server_thread_info *) thread_info);

  struct rtp_packet_t packet;
  struct sockaddr_in client_addr;
  packet = st->packet;
  client_addr = st->client_addr;

  
  // Get a connection_id
  st->list_elem.connection_id = packet.connection_id = pthread_self ();
  

  /* Copy Filename */
  char filename[PATH_LEN];
  strcpy (filename, packet.buffer);

  /* Send REQUEST_ACK */
  memset (packet.buffer, 0, PATH_LEN);
  packet.type = REQUEST_ACK;
  convert_packet (&packet, HTON);
  sendPacket (&packet, &socket_sem, &client_addr, socket_fd);

  // convert back to host to continue operation on the packet.
  convert_packet (&packet, NTOH);

  
  /* Store timestamp in packet buffer */
  fprintf (stderr, "Querying timestamp of %s\n", filename);
  time_t timestamp = get_timestamp (filename, packet.buffer);
  if (timestamp < 0)
  {
    // Error while retrieving timestamp. Set status to the error code
    packet.status = (uint16_t) timestamp;
  }
  else
  {
    packet.status = 0;
  }

  /* Send DONE */
  packet.type = (DONE);
  convert_packet (&packet, HTON);
  fprintf (stderr, "Sending timestamp of %s\n", filename);
  sendPacket (&packet, &socket_sem, &client_addr, socket_fd);

  /* TODO: Block until DONE_ACK received */

  // Lock mutex and then wait for signal to release mutex
  fprintf (stderr, "Child thread locking mutex.\n");
  pthread_mutex_lock( st->list_elem.mutex );
  fprintf (stderr, "Child thread mutex locked.\n");


//  fprintf (stderr, "Child thread waiting for signal.\n");
//  pthread_cond_wait( st->list_elem.condition_var, st->list_elem.mutex );
//  fprintf (stderr, "Child thread signalled, proceeding to release lock.\n");

  // Unlock mutex
  pthread_mutex_unlock( st->list_elem.mutex );

  // Lock n_threads_mutex
  pthread_mutex_lock (&n_threads_mutex);

  // Decrement the value;
  n_threads--;
  fprintf (stderr, "number of threads : %d\n", n_threads);
  
  // Release lock.
  pthread_mutex_unlock (&n_threads_mutex);

  LIST_REMOVE ( &(st->list_elem), entries);
  
  free (thread_info);
  fprintf (stderr, "Child thread received DONE_ACK. Exiting.\n");
  pthread_exit(0);
}
