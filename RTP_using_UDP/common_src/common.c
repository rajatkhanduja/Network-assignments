/* 
 * This file contains the definition of functions required by both 
 * the server and the client programs
 *
 * Author : Rajat Khanduja
 * Date : 15/1/2012
 */

#include <common.h>
#include <stdio.h>
#include <string.h> 

#define LOWER_BYTE(x)  x & ((unsigned char) -1)
#define HIGHER_BYTE(x) (x >> BYTE_SIZE) & ((unsigned char) -1)
  
void string_byte_order_convert (char *dest, char *src,
                                unsigned length, enum conversion conv_dir)
{
  /* Since the available function is ntohs, steps to perform
   * - take each character element
   * - check if its zero. If yes, there's no need to transform. 
   * - If not zero, type cast to uint16_t
   * - use ntohs and store the result in a uint16_t
   * - if first byte is zero, then second byte is the required one
   *   else the first byte itself is required.
   * - Store the required byte in the dest.
   */

  unsigned pos = 0;
  uint16_t character_tmp;
  unsigned i;

  enum { UNTESTED, LOWER, HIGHER } reqd_byte = UNTESTED;

  for (i = 0; i < length; i++)
  {
    if ( 0 == *(src + i) )
    {
      *(dest + i) = *(src + i);
      
      // Since this is NULL, no more copying required, break
      break;

    }
    else
    {
      character_tmp = (uint16_t) *(src + i);
      character_tmp = (conv_dir == NTOH) ? ntohs (character_tmp) 
                                         : htons (character_tmp);

      if ( LOWER == reqd_byte )
      {
        *(dest + i) = LOWER_BYTE (character_tmp);
      }

      else if ( HIGHER == reqd_byte )
      {
        *(dest + i) = HIGHER_BYTE (character_tmp);
      }

      else
      {
      
        *(dest + i) = LOWER_BYTE (character_tmp);
        if ( *(dest + i) == 0 )
        {
          // HIGHER BYTE (first 8 bits) is required).
          *(dest + i) = HIGHER_BYTE (character_tmp);
          reqd_byte = HIGHER;
        }
        else
        {
          // LOWER BYTE is required
          reqd_byte = LOWER;
        }
      }
    }
  }
}

void convert_packet (struct rtp_packet_t *packet, enum conversion conv_dir)
{
  if (conv_dir == NTOH)
  {
#define CONV(var) var = ntohs ((uint16_t) var);
  }
  else
  {
#define CONV(var) var = ntohs ((uint16_t) var);
  }

  CONV (packet->connection_id);
  CONV (packet->type);
  CONV (packet->status);

  char tmp[PATH_LEN];
  string_byte_order_convert (tmp, packet->buffer, PATH_LEN, conv_dir);

  strcpy (packet->buffer, tmp);
}

