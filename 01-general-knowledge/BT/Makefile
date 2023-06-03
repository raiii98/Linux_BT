.PHONY: obj all clean

PRJ_NAME = hello
SHARED_LIB_NAME = libhello.so
SATATIC_LIB_NAME = libhello.a
CUR_DIR = .
INC_DIR = $(CUR_DIR)/inc
SRC_DIR = $(CUR_DIR)/src
OBJ_DIR = $(CUR_DIR)/obj
SHARED_DIR = $(CUR_DIR)/lib/shared
STATIC_DIR = $(CUR_DIR)/lib/static
BIN_DIR = $(CUR_DIR)/bin

CC = gcc
FLAG = -c -fPIC -o

obj:
	$(CC) $(FLAG) $(OBJ_DIR)/hello.o $(SRC_DIR)/hello.c -I .
	$(CC) $(FLAG) $(OBJ_DIR)/main.o main.c -I .
	$(CC) $(FLAG) $(OBJ_DIR)/helloworld.o $(SRC_DIR)/helloworld.c -I .


shared: 
	$(CC) -shared -o $(SHARED_DIR)/$(SHARED_LIB_NAME) $(OBJ_DIR)/*.o
static:
	ar rcs $(STATIC_DIR)/$(SATATIC_LIB_NAME) $(OBJ_DIR)/*.o
all: 
	$(CC) $(OBJ_DIR)/main.o -L$(STATIC_DIR) -lhello -o $(BIN_DIR)/$(PRJ_NAME)
cleanobj: 
	rm -rf $(OBJ_DIR)/*

cleanhello:
	rm -rf hello
cleansharedlib:
	rm -rf $(SHARED_DIR)/*
clean:
	rm -rf $(BIN_DIR)/*