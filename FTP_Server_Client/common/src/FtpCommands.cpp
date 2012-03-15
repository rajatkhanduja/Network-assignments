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

string * dir (const string& directory)
{
	dirent * dp;
	DIR *dfd;
	string * response = new string();
  struct stat info;

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
		
		(*response) += "\n";
		(*response) += dp->d_name;

    if ( lstat (dp->d_name, &info) )
    {
      perror ("lstat");
    }
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
    std::cerr << "Couldn't open file " << fileName << " check\n";
		return NULL;
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

