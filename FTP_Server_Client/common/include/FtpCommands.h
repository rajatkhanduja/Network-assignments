/* This file holds the declaration of all the commands to be executed on the 
 * server. 
 */
#ifndef FTP_COMMANDS
#define FTP_COMMANDS

#include <string>
#include <fstream>
#include <list>

using std::string;
using std::ifstream;
using std::list;

/* Each command should return a single string as a reply. */

list <string> dir ( const string& directory, const bool& onlyRegularFile, const bool& recursive);
string * dir (const string& directory);
ifstream * getFileStream (const string& fileName);
bool putFileStream (const string& filename, const string& data);

// Auxillary functions
int replaceSpaces (string& list);    // True if replacements occur.

#endif	// End of file
