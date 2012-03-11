/* File containing header elements required by both the client and server
 * application. Constants, enums and macros.
 */
#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED

#define ARG_ERROR_MSG(string) std::cerr << argv[0] << " : " << optarg << " : " << string;
enum Errors {
  UnrecognizedArg = 1,
  MissingArg,
  NoHost,
  PortUnrecognized,
  InvalidQueueLength
};


#endif // End of file
