/* This file contains the definition of the functions to operate on thread_lists
 *
 * Author : Rajat Khanduja
 * Date : 16/1/2012
 */

#include <threads_list.h>

struct thread_list_elem * thread_find (uint16_t connection_id, struct thread_list *head)
{
  struct thread_list_elem *ptr;

  for ( ptr = head->lh_first; ptr != NULL; ptr = ptr->entries.le_next)
  {
    if (connection_id == ptr->connection_id)
    {
      return ptr;
    }
  }

  return NULL;
}
