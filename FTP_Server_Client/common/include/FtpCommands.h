/* This file holds the declaration of all the commands to be executed on the 
 * server. 
 */
#ifndef FTP_COMMANDS
#define FTP_COMMANDS

#include <string>
#include <fstream>
#include <list>
#include <TcpSocket.h>

using std::string;
using std::ifstream;
using std::list;

/* Function to return a list of file and directory names. Directory names 
 * end with a '/' whereas file names do not.
 * One could use this function to return a list of regular files in a folder
 * or to recursively list all those files.
 */
list <string> dir ( const string& directory, const bool& onlyRegularFile, const bool& recursive);

/* Function to return a string of regular files in a directory separated by
 * new-line character.
 */
string * dir (const string& directory);

/* Function to return the ifstream pointer obtained by opening the file
 * fileName. 
 */
ifstream * getFileStream (const string& fileName);

/* Function to dump the string data into the file filename. */
bool putFileStream (const string& filename, const string& data);

// Auxillary functions

/* This function replaces the spaces in the string list, except when the 
 * space is preceded by a backslash. 
 */
int replaceSpaces (string& list);    // Returns the number of replacements.

/* Function to send data of a file through the network. This 
 * assumes a list of filenames (with or without wild-card character and 
 * sends the data over the given socket. Based on the 'recursive' variable
 * it can send data recursively.
 *
 * Function returns a string of newline separated list of files that couldn't be sent.
 */
string sendFileData (const string& arg, const bool& recursive, TcpSocket& socket);

/* Function to receive files from the network sent by using the above
 * function. This function returns the list of files that have error. 
 * 
 * The error files makes sense on the client-side only as they are invalid
 * arguments for the command.
 */
string recvFileData (TcpSocket * socket);

inline bool isDir (const string& filename)
{
  return ( filename[filename.length() - 1] == '/');
}

#endif	// End of file
