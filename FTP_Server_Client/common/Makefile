SRC = src
OBJ = obj
CC  = g++
CFLAGS = -Iinclude/
OBJECTS = $(OBJ)/TcpSocket.o $(OBJ)/FtpClient.o $(OBJ)/FtpServer.o

all: $(OBJECTS)

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CC) -c -g $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ)/*.o

