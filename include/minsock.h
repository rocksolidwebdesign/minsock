#ifndef _MINSOCK_HEADER_DEFS
#define _MINSOCK_HEADER_DEFS

#ifdef __GNUC__
#   ifdef __MINGW32__
#       define WINDOWS
#   else
#       define LINUX
#   endif
#else
#   define WINDOWS
#endif

#ifdef WINDOWS
#   include <winsock2.h>
#   include <windows.h>
#   include <ws2tcpip.h>
#   ifdef __MINGW32__
#       include <unistd.h>
#   endif
#endif

#ifdef MINSOCK_DLL
#    ifdef MINSOCK_EXPORT
#        define DLLIFACE __declspec(dllexport)
#    else
#        define DLLIFACE __declspec(dllimport)
#    endif
#else
#    define DLLIFACE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef WINDOWS
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

DLLIFACE void minsock_err(int result, const char *msg);

DLLIFACE MINSOCKET* minsock_init(const char *host, const char *port);
DLLIFACE MINSOCKET* minsock_new();

DLLIFACE void minsock_host(MINSOCKET *s, const char *host, const char *port);

DLLIFACE MINSOCKET* minsock_open_connection(const char *host, const char *port);
DLLIFACE MINSOCKET* minsock_start_server(const char *host, const char *port);
DLLIFACE void minsock_connect_to(MINSOCKET *s, const char *host, const char *port);
DLLIFACE void minsock_listen_to(MINSOCKET *s, const char *host, const char *port);
DLLIFACE void minsock_connect(MINSOCKET *s);
DLLIFACE void minsock_bind(MINSOCKET *s);
DLLIFACE void minsock_listen(MINSOCKET *s);
DLLIFACE MINSOCKET* minsock_accept(MINSOCKET *s);

DLLIFACE void minsock_recv(MINSOCKET *s, char **response);
DLLIFACE void minsock_send(MINSOCKET *s, const char *request);

DLLIFACE void minsock_close(MINSOCKET *s);
DLLIFACE void minsock_free(MINSOCKET *s);
DLLIFACE void minsock_destroy(MINSOCKET *s);

DLLIFACE void minsock_resolve(char **ip, const char *hostname);

DLLIFACE char* minsock_strnew();
DLLIFACE void minsock_stradd(char **message, const char *append);
DLLIFACE void minsock_strset(char **message, const char *source);
DLLIFACE void minsock_getline(char **result);

DLLIFACE void minsock_win32_init(void);
DLLIFACE void minsock_win32_down(void);

#endif
