namespace Ftp
{
  const int defaultPort    = 8765;
	const int defaultBackLog = 20;
  enum ResponseCodes
  {
    Accept = 1,
    InvalidCommand,
    TempReject,
    PermReject
  };
}

