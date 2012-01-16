/*
 * This file contains the constants and functions to be used by both client and 
 * server.
 * 
 * Author : Rajat Khandujda
 * Date : 13/1/2012
 */

#ifndef COMMON_SERVER_CLIENT_H
#define COMMON_SERVER_CLIENT_H

#include <stdlib.h>     // For atoi and itoa
#include <assert.h>     // for assert
#include <arpa/inet.h>  // For ntohs

#define SERVER_PORT 1613  // Server port
#define PATH_LEN 256      // Actual length of path should be one less than this.

/* Define structure to store RTP packets */
struct rtp_packet_t 
{
  /* All shorts are defined to be 16-bit, signed integers. (in the network)*/
  int16_t connection_id;
  int16_t type;
  int16_t status;
  char buffer[PATH_LEN];
};

/* Other constants to be used to manipulate packets */
#define BUFFER_SIZE (2+2+2+256)     // != sizeof (struct rtp_packet_t)
#define PACKET_SHORT_SIZE 2         //  sizeof short in the packet
#define BYTE_SIZE 8

/* Enum to represent direction of conversion for packet conversion. */
enum conversion { NTOH, HTON };

/* Enum to represent the Packet type. */
enum packet_type { REQUEST, REQUEST_ACK, DONE, DONE_ACK};

/* 
 * Function to copy the 'character' array in network order to a string in
 * host order
 *
 */
void string_byte_order_convert (char *dest, char *src,
                                unsigned length, enum conversion conv_dir);

void convert_packet (struct rtp_packet_t *packet, enum conversion conv_dir);

#endif  // End of file
