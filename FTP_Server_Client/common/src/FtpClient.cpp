/* File containing the definition of FtpClient Class and its functions. */

#include <FtpClient.h>

bool FtpClient::connectToHost (const string& host, const int& port)
{
  return commandPort.connect (host, port);
}

TcpSocket::Errors FtpClient::getErrorVal () const
{
  return commandPort.getErrorVal();
}
