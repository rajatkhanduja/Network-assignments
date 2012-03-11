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
		CommandEnd
	};

  enum ResponseCodes
  {
    Accept = 1,
    InvalidCommand,
    TempReject,
    PermReject
  };

	inline bool isValidCommand (int command)
	{
		return ( (command >= CommandStart && command <= CommandEnd ));
	}
}

