#include <FtpCommands.h>
#include <dirent.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>

using std::ofstream;
using std::stringstream;

list<string> dir (const string& directory, const bool& onlyRegularFiles, const bool& recursive)
{
  dirent * dp;
  DIR *dfd;
  list<string> filenames;
  string path;

  struct stat info;

  // check if the directory exists and/or can be opened.
  if ( (dfd = opendir (directory.c_str())) == NULL )
  {
    return filenames;
  }

  // list the files in the directory
  while ( (dp = readdir (dfd)) != NULL )
  {
    // skip "." and ".." entries.
    if (strcmp (dp->d_name, "." ) == 0
      || strcmp (dp->d_name, "..") == 0 )
    {
      continue;
    }

    path = directory + string (dp->d_name);

    stat (path.c_str(), &info);
    if (S_ISREG (info.st_mode))
    {
      std::cerr << "Is a regular file :" << dp->d_name << "\n";
      filenames.push_back (path);
    }
    else if ( !onlyRegularFiles && S_ISDIR (info.st_mode) )
    {
      // Check if the path indicates its a directory.
      if (!isDir(path))
      {
        // If not, add a slash.
        path += "/";
      }

      filenames.push_back (path);

      if ( recursive )
      {
        list <string> tmp = dir ( string(dp->d_name), false, true);
        list <string>::iterator itr, itr_end;

        for ( itr = tmp.begin(), itr_end = tmp.end(); itr != itr_end; itr++)
        {
          filenames.push_back (*itr);
        }
      }
    }
  }

  return filenames;
}

string * dir (const string& directory)
{
	dirent * dp;
	DIR *dfd;
	string * response = new string();

	if ( (dfd = opendir (directory.c_str())) == NULL )
	{
		*response += "Can't open directory : ";
		*response += directory;
		return response;
	}

	while ( (dp = readdir (dfd)) != NULL)
	{
		if (strcmp (dp->d_name, ".")  == 0 
		 || strcmp (dp->d_name, "..") == 0)
		{
			continue;
		}
		
		(*response) += dp->d_name;
		(*response) += "\n";
	}

	closedir (dfd);

	return response;
}

ifstream * getFileStream (const string& fileName)
{
	ifstream * fileRead = new ifstream (fileName.c_str(), std::ios::in | std::ios::binary);

	if ( fileRead->is_open ())
	{
		return fileRead;
	}
	else
	{
    std::cerr << "Couldn't open file " << fileName << "\n";
    delete fileRead;
		return 0;
	}
}

int replaceSpaces (string& list)
{
  int offset = 0, count = 0;

  while ( (offset = list.find_first_of (' ', offset)) != string::npos)
  {
    if ( offset >0)
    {
      if (list[offset - 1] != '\\')
        list[offset] = '\n'; 
      else
      {
        list.erase (offset - 1, 1);
        offset--;
      }
    }

    offset++;
    count++;
  }

  return count;
}

bool putFileStream (const string& filename, const string& data) 
{
  ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
  
  if ( file.is_open())
  {
    file << data;
    return true;
  }
  else
  {
    return false;
  }
  
}

