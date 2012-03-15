// This file defines the functions required for the functions and variables.

#include <interface.h>
#include <sstream>
#include <iostream>

using std::istringstream;


string handleInput (string& userInput, FtpClient& client)
{
  string command, arguments;

  // Separate the command string and the arguments using stringstream.
  istringstream tokenizer (userInput);

  tokenizer >> command;
  getline (tokenizer, arguments);

  while (arguments[0] == ' ')
  {
    arguments.erase (0, 1);
  }

  std::cerr << command << " - " << arguments << std::endl;

  // Use the command string to call respective functions.
  if ( ! command.compare ("cd"))
  {
    // Execute 'cd' command
    return ( client.changeDir (arguments) ? "Success\n" : "Failed\n" );
  }
  else if ( ! command.compare ("lcd"))
  {
    // Execute lcd
    return ( client.changeLocalDir (arguments) ? "Success\n" : "Failed\n" );
  }
  else if ( ! command.compare ("get") || ! command.compare ("mget") )
  {
    // Execute 'get' command
    return (client.getFiles (arguments) ? "Command successful\n" : "Command failed\n");
  }
  else if ( ! command.compare ("put") || ! command.compare ("mput") )
  {
    // Execute 'put' command
    return (client.putFiles (arguments) ? "Command successful\n" : "Command failed\n");
  }
  else if ( ! command.compare ("dir") )
  {
    // Execute 'dir' command
    return client.listDir (arguments);
  }
  else if ( ! command.compare ("ldir"))
  {
    // Execute 'ldir' command
    return client.listLocalDir (arguments);
  }
  else if ( ! command.compare ("rget"))
  {
    // Execute 'rget' command
//    return client.getFiles (arguments, true);
  }
  else if ( ! command.compare ("rput"))
  {
    // Execute 'rput' command
//    return client.putFiles (arguments, true);
  }
  else if ( ! command.compare ("pwd"))
  {
    // Execute 'pwd' command
    return client.pwd ();
  }
  else if ( ! command.compare ("lpwd"))
  {
    return client.lpwd ();
  }
  else
  {
    // Unknown command. Report error.
    return string ("Unknown command" + command);
  }



  // Print the value or the error message that is returned.
}
