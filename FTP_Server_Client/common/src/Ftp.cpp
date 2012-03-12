/* This programs contains the definitions of the functions declared in 
 * Ftp.h 
 */
#include <Ftp.h>


string Ftp::response (int received)
{
  switch ( received )
  {
    case Ftp::Accept :
            return string ("Accepted\n");
    case Ftp::InvalidArg :  
            return string ("Invalid Argument\n");
    case Ftp::PermReject :
            return string ("Server refusing connection.\n");

    case Ftp::TempReject:
            return string ("Server temporarily refusion connection. Try later.\n");

    case Ftp::InvalidCommand:
            return string ("Invalid Command\n");
    default :
            return string ("Unexpected response\n");
  }
}

