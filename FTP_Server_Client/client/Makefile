SRC = src
OBJ = obj
BIN = bin
COMMON = ../common
COMMON_OBJ = $(COMMON)/obj
CC = g++
CFLAGS = -I../common/include -Iinclude/
OBJECTS = ${COMMON_OBJ}/FtpClient.o ${OBJ}/main.o

all: common ${OBJECTS}
	${CC} $^ -o ${BIN}/client
  
${OBJ}/%.o : ${SRC}/%.cpp
	${CC} -c -g ${CFLAGS} $< -o $@

common:
	make -C $(COMMON) all 

common_clean : clean
	make -C $(COMMON) clean

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*
