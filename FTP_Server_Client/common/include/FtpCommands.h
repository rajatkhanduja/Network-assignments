/* This file holds the declaration of all the commands to be executed on the 
 * server. 
 */
#ifndef FTP_SERVER_COMMANDS
#define FTP_SERVER_COMMANDS

#include <string>

using std::string;

/* Each command should return a single string as a reply. */

string * dir (const string& directory);


#endif	// End of file
