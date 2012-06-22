#ifndef _QUIRC_GLOBAL_INCLUDES
#define _QUIRC_GLOBAL_INCLUDES

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#ifdef __MINGW32__
#include <unistd.h>
#include <ws2tcpip.h>
#endif

#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#endif

#endif
