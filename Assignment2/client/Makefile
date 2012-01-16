SRC = src
OBJ = obj
BIN = bin
COMMON_SRC = ../common_src
COMMON_OBJ = ../common_obj
CC = gcc
CFLAGS = -I../common_include -Iinclude/
OBJECTS = ${COMMON_OBJ}/common.o ${OBJ}/main.o

all: ${OBJECTS}
	${CC} $^ -o ${BIN}/client
  
${OBJ}/%.o : ${SRC}/%.c
	${CC} -c -g ${CFLAGS} $< -o $@

${COMMON_OBJ}/%.o : ${COMMON_SRC}/%.c
	${CC} -c -g ${CFLAGS} $< -o $@

common_clean : clean
	rm -f ${COMMON_OBJ}/*.o

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*
