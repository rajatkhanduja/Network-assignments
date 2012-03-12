#include <FtpCommands.h>
#include <dirent.h>
#include <cstring>
#include <fstream>

using std::ifstream;

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
		
		(*response) += "\n";
		(*response) += dp->d_name;
	}

	closedir (dfd);

	return response;
}

ifstream * getFile (const string& fileName)
{
	ifstream * fileRead = new ifstream (fileName.c_str());

	if ( fileRead->is_open ())
	{
		return fileRead;
	}
	else
	{
		return NULL;
	}
}	
