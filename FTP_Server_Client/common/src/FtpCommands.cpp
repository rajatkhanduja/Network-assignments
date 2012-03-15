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
using std::istringstream;


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

    std::cerr <<"testing file " << path <<  " " << onlyRegularFiles << S_ISDIR (info.st_mode) << std::endl;

    stat (path.c_str(), &info);
    if (S_ISREG (info.st_mode))
    {
      std::cerr << "Is a regular file :" << path << "\n";
      filenames.push_back (path);
    }
    else if ( !onlyRegularFiles && S_ISDIR (info.st_mode) )
    {
      std::cerr << "Is directory : " << path << std::endl;
      // Check if the path indicates its a directory.
      if (!isDir(path))
      {
        // If not, add a slash.
        path += "/";
      }

      filenames.push_back (path);

      if ( recursive )
      {
        list <string> tmp = dir ( path, false, true);
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

bool transmitFile (const string& filename, TcpSocket& socket)
{
  /* Returns filename if it couldn't be opened. */
  ifstream *fileStream = getFileStream (filename);

  if ( fileStream != NULL)
  {
    std::cerr << "File opened. " << filename << " " << fileStream << std::endl ;
    socket << filename;
    socket << (*fileStream);
    fileStream->close();
    return true;
  }
  else
  {
    return false;
  }
}

string sendFileData (const string& arg, const bool& recursive, TcpSocket& socket)
{
  istringstream tmpStream(arg);
  string token;
  list<string> filenames;
  bool readList;
  
  std::cerr << "Beginning processing" << std::endl;
              
  string errFiles;
  while ( std::getline(tmpStream, token, '\n'))
  {
    std::cerr << "Getting file " << token << std::endl;
    int pos;
    if ( (pos = token.find ('*')) != string::npos || recursive  )
    {
      // Only complete directories supported.
      if (pos == token.length() - 1 ) 
      {
        token.erase (pos, 1);
      }

      filenames = dir (token, (!recursive), recursive );
                
      list<string>::iterator itr, itr_end;
      for (itr = filenames.begin(), itr_end = filenames.end(); itr != itr_end ; itr++)
      {
        if ((*itr)[itr->length() - 1] == '/')
        {
          socket << (*itr);
          socket << string();
        }
        else
          transmitFile (*itr, socket); 
      }
    }
    else if ( !transmitFile (token, socket) )
    {
      errFiles += string(token); 
      errFiles += "\n";
    }
  }
 
  return errFiles;
}


