#include "global.h"
#include "minsock.h"

void minsock_close(minsock_fd *s)
{
#ifdef _WIN32
    closesocket(*s);
#else
    close(*s);
#endif
}

void minsock_mksocket(minsock_fd *s)
{
    *s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    minsock_err(*s, "Could not open the socket");
}

void minsock_mkaddr(minsock_addr *a, char *hostname, char *port)
{
    char ip[100];

    printf("Resolving hostname '%s'\n", hostname);

    minsock_resolve(hostname, ip);
    printf("Listening On %s:%s\n", ip, port);

    a->sin_family = AF_INET;

    /* set the host */
#ifdef _WIN32
    a->sin_addr.s_addr = inet_addr(ip);
#else
    inet_aton(ip, &a->sin_addr);
#endif

    /* set the port */
    a->sin_port = htons(atoi(port));
}

void minsock_resolve(char *hostname, char *ip)
{
    struct hostent *host_info;
    struct in_addr **addr_list;
    int i;

    if ((host_info = gethostbyname(hostname)) == NULL)
    {
        // get the host info
        printf("Could not resolve hostname '%s'\n", hostname);
        exit(errno);
    }

    addr_list = (struct in_addr **) host_info->h_addr_list;

    for (i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return;
    }

    exit(1);
}

void minsock_bind(minsock_fd *s, minsock_addr *a)
{
    int result;
    socklen_t len;

    len = sizeof(*a);
    result = bind(*s, (struct sockaddr*)a, len);

    minsock_err(result, "Could not bind the socket");
}

void minsock_listen(minsock_fd *s)
{
    int result = listen(*s, 5);

    minsock_err(result, "Could not listen for connections");
}

void minsock_accept(minsock_fd *s, minsock_fd *n, minsock_addr *client_address, socklen_t *client_address_length)
{
    *n = accept(*s, (struct sockaddr*)client_address, client_address_length);

    minsock_err(*n, "Could not accept socket connection");

    fprintf(stdout, "Established socket connection.\n");
}

void minsock_connect(minsock_fd *s, minsock_addr *server_address, socklen_t *server_address_length)
{
    int result;

    result = connect(*s, (struct sockaddr*)server_address, *server_address_length);

    minsock_err(result, "Could not connect to server");

    fprintf(stdout, "Established socket connection.\n");
}

void minsock_recv(minsock_fd *s, char *result, char *until)
{
    int buf_size = 256;
    int status   = -1;
    char buffer[buf_size];

    do
    {
        memset(buffer, 0, buf_size);
        status = recv(*s, buffer, buf_size-1, 0);
        minsock_err(status, "Could not read socket");

        strcat(result, buffer);
    }
    while (strstr(buffer, until) == NULL);

    fprintf(stdout, "Socket read successful.\n");
}

void minsock_send(minsock_fd *s, char *message)
{
    int result;

    result = send(*s, message, strlen(message), 0);

    minsock_err(result, "Could not write socket");

    fprintf(stdout, "Socket write successful.\n");
}

void minsock_get_until(char *result, char *until)
{
    char buffer[256];

    do
    {
        memset(buffer, 0, 256);

        if (fgets(buffer, 255, stdin) == NULL)
        {
            fprintf(stdout, "Error reading input.\n");
            return;
        }

        strcat(result, buffer);
    }
    while (strstr(buffer, until) == NULL);
}

void minsock_win32_init(void)
{
#ifdef _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 2);

    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
        printf("Cannot find a usable winsock.dll");
        return;
    }

    printf("Version %d.%d\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

    if (
      LOBYTE(wsaData.wVersion) != 2 ||
      HIBYTE(wsaData.wVersion) != 2
    ) {
        printf("Cannot find a usable winsock.dll");
        WSACleanup();
        return;
    }
#endif
}

void minsock_win32_down(void)
{
#ifdef _WIN32
    WSACleanup();
#endif
}

void minsock_err(int result, char *msg)
{
    if (result < 0)
    {
        fprintf(stdout, "%s. ERROR %d: %s\n", msg, errno, strerror(errno));
        exit(errno);
    }
}

