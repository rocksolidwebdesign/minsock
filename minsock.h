#ifndef _MINSOCK_HEADER_DEFS
#define _MINSOCK_HEADER_DEFS

#include "global.h"

#ifdef _WIN32
    typedef SOCKET minsock_fd;
    typedef SOCKADDR_IN minsock_addr;
#else
    typedef int minsock_fd;
    typedef struct sockaddr_in minsock_addr;
#endif

void minsock_close(minsock_fd *s);
void minsock_resolve(char *hostname , char *ip);
void minsock_mksocket(minsock_fd *s);
void minsock_mkaddr(minsock_addr *a, char *hostname, char *port);
void minsock_bind(minsock_fd *s, minsock_addr *a);
void minsock_listen(minsock_fd *s);
void minsock_accept(minsock_fd *s, minsock_fd *n, minsock_addr *client_address, socklen_t *client_address_length);
void minsock_connect(minsock_fd *s, minsock_addr *server_address, socklen_t *server_address_length);
void minsock_recv(minsock_fd *s, char *result, char *until);
void minsock_send(minsock_fd *s, char *message);
void minsock_get_until(char *result, char *until);
void minsock_win32_init(void);
void minsock_win32_down(void);

/* private */
void minsock_err(int result, char *msg);

#endif
