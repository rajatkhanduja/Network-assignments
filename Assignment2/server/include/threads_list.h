/* Program contains the structure and function declarations for creating 
 * and handling a list of threads. This thread is required for maintaining 
 * various threads on a server.
 *
 * Author : Rajat Khanduja
 * Date : 16/1/2012
 */

#ifndef THREAD_LIST_INCLUDED
#define THREAD_LIST_INCLUDED

#include <pthread.h>
#include <sys/queue.h>
#include <stdint.h>

LIST_HEAD (thread_list, thread_list_elem) head;

struct thread_list_elem
{
  pthread_t thread;
  LIST_ENTRY(thread_list_elem) entries;
  uint16_t connection_id;
  pthread_mutex_t *mutex;
  pthread_cond_t *condition_var;
};

struct thread_list_elem * thread_find (uint16_t connection_id, struct thread_list *head);

#endif // End of file
