CXX_CC=g++
CC_OPTS=-Wall -g
SHARED_OPTS=-fPIC -shared
OBJ_DIR=lib
OUT_DIR=bin
INCLUDE_PREFIX=/usr/local/include
LIB_PREFIX=/usr/local/lib

WIN32_CC=i586-mingw32msvc-gcc
WIN32_CC_OPTS=-g -Wall

all: library examples tests

library: libminsockcxx.so

examples: server client serverxx clientxx

tests: testcc testxx

libminsockcxx.so: libminsock.so minsock.hpp
	$(CXX_CC) $(CC_OPTS) $(SHARED_OPTS) ./minsock.cpp -o ./$(OBJ_DIR)/libminsockcxx.so

libminsock.so: minsock.c minsock.h
	$(CC) $(CC_OPTS) $(SHARED_OPTS) ./minsock.c -o ./$(OBJ_DIR)/libminsock.so

testcc:
	$(CC) $(CC_OPTS) ./test/test.c -o ./$(OUT_DIR)/test -lminsock
testxx:
	$(CXX_CC) $(CC_OPTS) ./test/test.cpp -o ./$(OUT_DIR)/testxx -lminsock

server:
	$(CC) $(CC_OPTS) ./test/server.c -o ./$(OUT_DIR)/server -lminsock
client:
	$(CC) $(CC_OPTS) ./test/client.c -o ./$(OUT_DIR)/client -lminsock

serverxx:
	$(CXX_CC) $(CC_OPTS) ./test/server.cpp -o ./$(OUT_DIR)/serverxx -lminsock -lminsockcxx
clientxx:
	$(CXX_CC) $(CC_OPTS) ./test/client.cpp -o ./$(OUT_DIR)/clientxx -lminsock -lminsockcxx

install: uninstall
	cp ./$(OBJ_DIR)/libminsock.so /usr/local/lib
	cp ./$(OBJ_DIR)/libminsockcxx.so /usr/local/lib
	mkdir -p $(INCLUDE_PREFIX)/minsock
	cp minsock.h $(INCLUDE_PREFIX)/
	cp minsock.hpp $(INCLUDE_PREFIX)/
	ln -s $(LIB_PREFIX)/libminsock.so $(LIB_PREFIX)/libminsock.so.0
	ln -s $(LIB_PREFIX)/libminsock.so $(LIB_PREFIX)/libminsock.so.0.0.1
	ln -s $(LIB_PREFIX)/libminsockcxx.so $(LIB_PREFIX)/libminsockcxx.so.0
	ln -s $(LIB_PREFIX)/libminsockcxx.so $(LIB_PREFIX)/libminsockcxx.so.0.0.1
	ldconfig

uninstall:
	rm -rf $(LIB_PREFIX)/libminsock*
	rm -rf $(INCLUDE_PREFIX)/minsock*
	ldconfig

#windows: minsock.a
#	$(WIN32_CC) $(CC_OPTS) -shared -o $(OBJ_DIR)/minsock.dll $(OBJ_DIR)/minsock.a -lws2_32
#
#minsock.a: minsock.c minsock.h
#	$(WIN32_CC) $(CC_OPTS) -c minsock.c -o $(OBJ_DIR)/minsock.a -lws2_32

clean:
	rm -rf ./$(OUT_DIR)/client* ./$(OUT_DIR)/server* ./$(OUT_DIR)/test* ./$(OBJ_DIR)/*.o ./$(OBJ_DIR)/*.so*
