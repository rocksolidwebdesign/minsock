#ifndef _MINSOCK_HEADER_DEFS
#define _MINSOCK_HEADER_DEFS

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

#ifdef _WIN32
    typedef SOCKET minsock_fd;
    typedef SOCKADDR_IN minsock_addr;
#else
    typedef int minsock_fd;
    typedef struct sockaddr_in minsock_addr;
#endif

typedef struct minsock_class_t
{
    char *until;

    char *request;
    char *response;

    char *host;
    char *port;

    char *ip;

    /* the actual socket file descriptor */
    minsock_fd *connection;

    minsock_addr *address;
    socklen_t *address_length;
}
MINSOCKET;

void minsock_err(int result, const char *msg);

MINSOCKET* minsock_init(const char *host, const char *port);
MINSOCKET* minsock_new();

void minsock_host(MINSOCKET *s, const char *host, const char *port);

MINSOCKET* minsock_open_connection(const char *host, const char *port);
MINSOCKET* minsock_start_server(const char *host, const char *port);
void minsock_connect_to(MINSOCKET *s, const char *host, const char *port);
void minsock_listen_to(MINSOCKET *s, const char *host, const char *port);
void minsock_connect(MINSOCKET *s);
void minsock_bind(MINSOCKET *s);
void minsock_listen(MINSOCKET *s);
MINSOCKET* minsock_accept(MINSOCKET *s);

void minsock_recv(MINSOCKET *s);
void minsock_send(MINSOCKET *s);

void minsock_close(MINSOCKET *s);
void minsock_free(MINSOCKET *s);
void minsock_destroy(MINSOCKET *s);

/* utility functions */
char * minsock_resolve(const char *hostname);
void minsock_getline(char *result);

/* winsock related stuff */
void minsock_win32_init(void);
void minsock_win32_down(void);

/* private */

#endif
