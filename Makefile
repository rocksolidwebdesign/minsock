CC=gcc
CXX=g++
CC_OPTS=-Wall -g -static-libgcc -static-libstdc++ -I./include/
CXX_OPTS=-Wall -g -static-libgcc -static-libstdc++ -I./include/
BUILD_DIR=build
BIN_DIR=bin
INCLUDE_PREFIX=/usr/local/include
LIB_PREFIX=/usr/local/lib

WIN32_CC=i586-mingw32msvc-gcc
WIN32_CXX=i586-mingw32msvc-g++
WIN32_CC_OPTS=-g -Wall

UNAME=$(shell uname)

ifeq ($(UNAME), MINGW32_NT-5.1)
# MINGW WINDOWS {{{
all: library examples tests

library: minsockxx.dll

examples: server serverxx client clientxx

tests: testc testxx

minsock.dll: minsock.c
	$(CC) $(CC_OPTS) -DMINSOCK_DLL -DMINSOCK_EXPORT -c      ./minsock.c -o ./$(BUILD_DIR)/minsock.o  -lws2_32
	$(CC) $(CC_OPTS) -DMINSOCK_DLL -DMINSOCK_EXPORT -shared ./$(BUILD_DIR)/minsock.o -o ./$(BIN_DIR)/minsock.dll -lws2_32

minsockxx.dll: minsock.dll
	$(CXX) $(CXX_OPTS) -DMINSOCK_DLL -DMINSOCK_EXPORT -c      ./minsock.cpp -o              ./$(BUILD_DIR)/minsockxx.o -L./$(BIN_DIR)/ -lminsock -lws2_32
	$(CXX) $(CXX_OPTS) -DMINSOCK_DLL -DMINSOCK_EXPORT -shared ./$(BUILD_DIR)/minsockxx.o -o ./$(BIN_DIR)/minsockxx.dll -L./$(BIN_DIR)/ -lminsock -lws2_32

server: minsock.dll
	$(CC) $(CC_OPTS) -DMINSOCK_DLL ./test/server.c -o ./$(BIN_DIR)/server.exe -L./$(BIN_DIR)/ -lminsock -lws2_32

serverxx: minsockxx.dll
	$(CXX) $(CXX_OPTS) -DMINSOCK_DLL ./test/server.cpp -o ./$(BIN_DIR)/serverxx.exe -L./$(BIN_DIR)/ -lminsock -lminsockxx -lws2_32

client: minsock.dll
	$(CC) $(CC_OPTS) -DMINSOCK_DLL ./test/client.c -o ./$(BIN_DIR)/client.exe -L./$(BIN_DIR)/ -lminsock -lws2_32

clientxx: minsockxx.dll
	$(CXX) $(CXX_OPTS) -DMINSOCK_DLL ./test/client.cpp -o ./$(BIN_DIR)/clientxx.exe -L./$(BIN_DIR)/ -lminsock -lminsockxx -lws2_32

testc: minsock.dll
	$(CC) $(CC_OPTS) -DMINSOCK_DLL ./test/test.c -o ./$(BIN_DIR)/test.exe -L./$(BIN_DIR)/ -lminsock -lws2_32

testxx: minsockxx.dll
	$(CXX) $(CXX_OPTS) -DMINSOCK_DLL ./test/test.cpp -o ./$(BIN_DIR)/testxx.exe -L./$(BIN_DIR)/ -lminsock -lminsockxx -lws2_32

clean:
	rm -rf ./$(BIN_DIR)/* ./$(BUILD_DIR)/*
# }}}
else
# LINUX {{{
APP_LIBS=-L./$(BUILD_DIR)/ -lminsock
APP_LIBSXX=-L./$(BUILD_DIR)/ -lminsock -lminsockcxx
SHARED_OPTS=-fPIC -shared
SHARED_NAME=libminsock.so
SHARED_NAMEXX=libminsockxx.so

all: library examples tests

library: libminsockcxx.so

examples: server client serverxx clientxx

tests: testcc testxx

libminsockcxx.so: libminsock.so minsock.hpp
	$(CXX) $(CC_OPTS) $(SHARED_OPTS) ./minsock.cpp -o ./$(BUILD_DIR)/$(SHARED_NAMEXX)

libminsock.so: minsock.c minsock.h
	$(CC) $(CC_OPTS) $(SHARED_OPTS) ./minsock.c -o ./$(BUILD_DIR)/$(SHARED_NAME)

testcc:
	$(CC) $(CC_OPTS) ./test/test.c -o ./$(BIN_DIR)/test $(APP_LIBS)
testxx:
	$(CXX) $(CC_OPTS) ./test/test.cpp -o ./$(BIN_DIR)/testxx $(APP_LIBS)

server:
	$(CC) $(CC_OPTS) ./test/server.c -o ./$(BIN_DIR)/server $(APP_LIBS)
client:
	$(CC) $(CC_OPTS) ./test/client.c -o ./$(BIN_DIR)/client $(APP_LIBS)

serverxx:
	$(CXX) $(CC_OPTS) ./test/server.cpp -o ./$(BIN_DIR)/serverxx $(APP_LIBSXX)
clientxx:
	$(CXX) $(CC_OPTS) ./test/client.cpp -o ./$(BIN_DIR)/clientxx $(APP_LIBSXX)

install: uninstall
	cp ./$(BUILD_DIR)/libminsock.so /usr/local/lib
	cp ./$(BUILD_DIR)/libminsockcxx.so /usr/local/lib
	mkdir -p $(INCLUDE_PREFIX)/minsock
	cp minsock.h $(INCLUDE_PREFIX)/
	cp minsock.hpp $(INCLUDE_PREFIX)/
	ln -s $(LIB_PREFIX)/libminsock.so $(LIB_PREFIX)/$(SHARED_NAME).0
	ln -s $(LIB_PREFIX)/libminsock.so $(LIB_PREFIX)/$(SHARED_NAME).0.0.1
	ln -s $(LIB_PREFIX)/libminsockcxx.so $(LIB_PREFIX)/$(SHARED_NAMEXX).0
	ln -s $(LIB_PREFIX)/libminsockcxx.so $(LIB_PREFIX)/$(SHARED_NAMEXX).0.0.1
	ldconfig

uninstall:
	rm -rf $(LIB_PREFIX)/$(SHARED_NAME)*
	rm -rf $(INCLUDE_PREFIX)/minsock*
	ldconfig

clean:
	rm -rf ./$(BIN_DIR)/client* ./$(BIN_DIR)/server* ./$(BIN_DIR)/test* ./$(BUILD_DIR)/*.o ./$(BUILD_DIR)/*.so*
# }}}
endif
