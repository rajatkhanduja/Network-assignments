#include <string>

using std::string;

namespace Ftp
{
  const int defaultPort    = 8765;
	const int defaultBackLog = 20;

/* Format of commands sent by the client should be as follows :-
 * 
 * CommandCode,Arg
 * 
 * ',' signifies the end of a byte. In other words, first element of the 
 * received string is always considered to be the command.
 * The rest of it is passed on as argument, until the EOF character is
 * encoutnered
 */

	enum CommandCodes
	{
		CommandStart = 1,
		Get,
		MGet,
		Put,
		MPut,
		Dir,
		Terminate,
		PortVal,
		ChDir,
		CommandEnd
	};

  enum ResponseCodes
  {
    Accept = 1,
    InvalidCommand,
    TempReject,
    PermReject,
    InvalidArg      /* Use in cases where the command is correct but the argument is incorrect. 
                     * For instance, a file that does not exist or cannot be opened.
                     */
  };

	inline bool isValidCommand (int command)
	{
		return ( (command >= CommandStart && command <= CommandEnd ));
	}

  string response (int received);

}

