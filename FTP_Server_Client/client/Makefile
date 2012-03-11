SRC = src
OBJ = obj
BIN = bin
COMMON = ../common
COMMON_LIB = $(COMMON)/obj/FtpServerClient.a
CC = g++
CFLAGS = -I$(COMMON)/include -Iinclude/ -I$(COMMON)
OBJECTS = ${OBJ}/main.o

all: common ${OBJECTS}
	${CC} $(OBJECTS) $(COMMON_LIB) -o ${BIN}/client
  
${OBJ}/%.o : ${SRC}/%.cpp
	${CC} -c -g ${CFLAGS} $< -o $@

common:
	make -C $(COMMON) all 

common_clean : clean
	make -C $(COMMON) clean

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*
