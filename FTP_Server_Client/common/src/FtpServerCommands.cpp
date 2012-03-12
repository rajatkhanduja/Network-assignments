#include <FtpServerCommands.h>

#include <dirent.h>
#include <cstring>

string * dir (const string& directory)
{
	dirent * dp;
	DIR *dfd;
	string * response = new string();

	if ( (dfd = opendir (directory.c_str())) == NULL )
	{
		return response;
	}

	while ( (dp = readdir (dfd)) != NULL)
	{
		if (strcmp (dp->d_name, ".")  == 0 
		 || strcmp (dp->d_name, "..") == 0)
		{
			continue;
		}
		
		(*response) += " ";
		(*response) += dp->d_name;
	}

	closedir (dfd);

	return response;
}
