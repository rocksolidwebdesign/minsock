CC=gcc
CC_OPTS=-g -Wall
OUT_DIR=bin
OBJ_DIR=lib

WIN32_CC=i586-mingw32msvc-gcc
WIN32_CC_OPTS=-g -Wall

all: linux

linux: server client

server: server.o minsock.o
	$(CC) $(WIN32_CC_OPTS) $(OBJ_DIR)/minsock.o $(OBJ_DIR)/server.o -o $(OUT_DIR)/server
client: client.o minsock.o
	$(CC) $(WIN32_CC_OPTS) $(OBJ_DIR)/minsock.o $(OBJ_DIR)/client.o -o $(OUT_DIR)/client
server.o: server.c minsock.h global.h
	$(CC) $(WIN32_CC_OPTS) -c server.c -o $(OBJ_DIR)/server.o
client.o: client.c minsock.h global.h
	$(CC) $(WIN32_CC_OPTS) -c client.c -o $(OBJ_DIR)/client.o
minsock.o: minsock.c minsock.h global.h
	$(CC) $(WIN32_CC_OPTS) -c minsock.c -o $(OBJ_DIR)/minsock.o

windows: server.exe client.exe

server.exe: server.a minsock.a
	$(WIN32_CC) $(CC_OPTS) $(OBJ_DIR)/server.a $(OBJ_DIR)/minsock.a -o $(OUT_DIR)/server.exe -lws2_32
client.exe: client.a minsock.a
	$(WIN32_CC) $(CC_OPTS) $(OBJ_DIR)/client.a $(OBJ_DIR)/minsock.a -o $(OUT_DIR)/client.exe -lws2_32
server.a: server.c minsock.h global.h
	$(WIN32_CC) $(CC_OPTS) -c server.c -o $(OBJ_DIR)/server.a -lws2_32
client.a: client.c minsock.h global.h
	$(WIN32_CC) $(CC_OPTS) -c client.c -o $(OBJ_DIR)/client.a -lws2_32
minsock.a: minsock.c minsock.h global.h
	$(WIN32_CC) $(CC_OPTS) -c minsock.c -o $(OBJ_DIR)/minsock.a -lws2_32

clean:
	rm -rf $(OUT_DIR)/* $(OBJ_DIR)/*
