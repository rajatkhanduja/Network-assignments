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
 */
//bool sendData (const string& 


inline bool isDir (const string& filename)
{
  return ( filename[filename.length() - 1] == '/');
}

#endif	// End of file
