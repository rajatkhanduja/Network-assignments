/* This file declares the interface related functions and variables. */
#ifndef CLIENT_INTERFACE_INCLUDED
#define CLIENT_INTERFACE_INCLUDED

#include <string>
#include <FtpClient.h>

using std::string;

/* Function to handle the input and execute the command requested by the user
 * Space acts like a delimiter unless it is preceded by a 'backslash'. */
string handleInput (string& userInput, FtpClient& client);


#endif // End of file
