SRC = src
OBJ = obj
CC  = g++
CFLAGS = -Iinclude/
AR = ar
LIB_OBJ = $(OBJ)/FtpServerClient.a
OBJECTS = $(OBJ)/Ftp.o $(OBJ)/FtpCommands.o $(OBJ)/TcpSocket.o $(OBJ)/FtpClient.o $(OBJ)/FtpServer.o

all: libServerClient

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CC) -Wall -c $(CFLAGS) $< -o $@

libServerClient: $(OBJECTS)
	$(AR) crs $(LIB_OBJ) $(OBJECTS) 

clean:
	rm -f $(OBJ)/*

