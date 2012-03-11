/* File containing the definition of FtpClient Class and its functions. */

#include <FtpClient.h>

bool FtpClient::connectToHost (const string& host, const int& port)
{
//  return commandPort.connect (host, port);
  if (commandPort.connect (host, port))
  {
    string test("test message\n");
    commandPort << test;
   
    // Wait for reply
    commandPort >> test;
    std::cerr << test;
    return true;
  }
  else return false;
}

TcpSocket::Errors FtpClient::getErrorVal () const
{
  return commandPort.getErrorVal();
}
