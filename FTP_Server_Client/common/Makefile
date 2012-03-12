SRC = src
OBJ = obj
CC  = g++
CFLAGS = -Iinclude/
AR = ar
LIB_OBJ = $(OBJ)/FtpServerClient.a
OBJECTS = $(OBJ)/TcpSocket.o $(OBJ)/FtpClient.o $(OBJ)/FtpServer.o $(OBJ)/FtpCommands.o

all: libServerClient

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CC) -Wall -c -g $(CFLAGS) $< -o $@

libServerClient: $(OBJECTS)
	$(AR) crs $(LIB_OBJ) $(OBJECTS) 

clean:
	rm -f $(OBJ)/*

