SRC = src
OBJ = obj
BIN = bin
COMMON = ../common
COMMON_LIB = $(COMMON)/obj/FtpServerClient.a
CC = g++
CFLAGS = -I$(COMMON)/include -Iinclude/ -I$(COMMON)
OBJECTS = $(OBJ)/interface.o ${OBJ}/main.o

all: common ${OBJECTS}
	${CC} $(OBJECTS) $(COMMON_LIB) -o ${BIN}/client.out
  
${OBJ}/%.o : ${SRC}/%.cpp
	${CC} -c ${CFLAGS} $< -o $@

common:
	make -C $(COMMON) all 

common_clean : clean
	make -C $(COMMON) clean

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*.out
