#include "minsock.h"

void minsock_err(int result, const char *msg)
{
    if (result < 0)
    {
        fprintf(stdout, "%s. ERROR %d: %s\n", msg, errno, strerror(errno));
        exit(errno);
    }
}

MINSOCKET* minsock_init(const char *host, const char *port)
{
    MINSOCKET *s;

    s = minsock_new();

    minsock_host(s, host, port);

    return s;
}

MINSOCKET* minsock_new()
{
    MINSOCKET *s;
    s = malloc(sizeof(*s));

    s->request        =  malloc(sizeof(*(s->request)));
    s->response       =  malloc(sizeof(*(s->response)));
    s->until          =  malloc(sizeof(*(s->until)));

    s->host           =  malloc(sizeof(*(s->host)));
    s->port           =  malloc(sizeof(*(s->port)));
    s->ip             =  malloc(sizeof(*(s->ip)));

    s->connection     =  malloc(sizeof(*(s->connection)));
    s->address        =  malloc(sizeof(*(s->address)));
    s->address_length =  malloc(sizeof(*(s->address_length)));

    *(s->address_length) = sizeof(*(s->address));
    strcpy(s->until, "\n");
    strcpy(s->response, "");
    strcpy(s->request, "");

    *(s->connection) = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    minsock_err(*(s->connection), "Could not open the socket");

    return s;
}

void minsock_host(MINSOCKET *s, const char *host, const char *port)
{
    strcpy(s->host, host);
    strcpy(s->port, port);

    s->ip = minsock_resolve(s->host);

    printf("Listening On %s:%s\n", s->ip, s->port);

    s->address->sin_family = AF_INET;

    /* set the host */
#ifdef _WIN32
    s->address->sin_addr.s_addr = inet_addr(s->ip);
#else
    inet_aton(s->ip, &s->address->sin_addr);
#endif

    /* set the port */
    s->address->sin_port = htons(atoi(s->port));
}

MINSOCKET* minsock_open_connection(const char *host, const char *port)
{
    MINSOCKET *s;

    s = minsock_init(host, port);
    minsock_connect(s);

    return s;
}

MINSOCKET* minsock_start_server(const char *host, const char *port)
{
    MINSOCKET *s;

    s = minsock_init(host, port);
    minsock_listen(s);

    return s;
}

void minsock_connect_to(MINSOCKET *s, const char *host, const char *port)
{
    minsock_host(s, host, port);
    minsock_connect(s);
}

void minsock_listen_to(MINSOCKET *s, const char *host, const char *port)
{
    minsock_host(s, host, port);
    minsock_listen(s);
}

void minsock_connect(MINSOCKET *s)
{
    int result;

    *(s->address_length) = sizeof(*(s->address));
    result = connect(*(s->connection), (struct sockaddr*)s->address, *(s->address_length));

    minsock_err(result, "Could not connect to server");

    fprintf(stdout, "Established socket connection.\n");
}

void minsock_bind(MINSOCKET *s)
{
    int result;

    *(s->address_length) = sizeof(*(s->address));
    result = bind(*(s->connection), (struct sockaddr*)s->address, *(s->address_length));

    minsock_err(result, "Could not bind the socket");
}

void minsock_listen(MINSOCKET *s)
{
    minsock_bind(s);

    int result = listen(*(s->connection), 5);

    minsock_err(result, "Could not listen for connections");
}

MINSOCKET* minsock_accept(MINSOCKET *s)
{
    MINSOCKET *n;
    n = minsock_new();

    *(n->address_length) = sizeof(*(s->address));
    *(n->connection) = accept(*(s->connection), (struct sockaddr*)n->address, n->address_length);

    minsock_err(*(s->connection), "Could not accept socket connection");

    fprintf(stdout, "Established socket connection.\n");

    return n;
}

void minsock_recv(MINSOCKET *s)
{
    int s = 256;
    int status;
    char buffer[s];

    strcpy(s->response, "");

    do
    {
        memset(buffer, 0, s);
        status = recv(*(s->connection), buffer, s-1, 0);
        minsock_err(status, "Could not read socket");

        minsock_strcat(s->response, buffer);
    }
    while (strstr(buffer, s->until) == NULL);

    fprintf(stdout, "Socket read successful.\n");
}

void minsock_send(MINSOCKET *s)
{
    int result;

    result = send(*(s->connection), s->request, strlen(s->request), 0);

    minsock_err(result, "Could not write socket");

    fprintf(stdout, "Socket write successful.\n");
}

void minsock_close(MINSOCKET *s)
{
#ifdef _WIN32
    closesocket(*(s->connection));
#else
    close(*(s->connection));
#endif
}

void minsock_free(MINSOCKET *s)
{
    free(s->request);
    free(s->response);
    free(s->until);

    free(s->host);
    free(s->port);
    free(s->ip);

    free(s->connection);
    free(s->address);
    free(s->address_length);

    free(s);
}

void minsock_destroy(MINSOCKET *s)
{
    minsock_close(s);
    minsock_free(s);
}

char * minsock_resolve(const char *hostname)
{
    char *ip;
    ip = malloc(sizeof(*ip));

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
        minsock_strcpy(ip, inet_ntoa(*addr_list[i]));
        return ip;
    }

    exit(1);
}

void minsock_stradd(char *message, char *append)
{
    message = realloc(message, strlen(message)+strlen(append)+1);
    strcat(message, append);
}

void minsock_strset(char *message, char *source)
{
    message = realloc(message, strlen(source)+1);
    strcpy(message, source);
}

void minsock_getline(char *result)
{
    int s = 256;
    char buffer[s];

    minsock_strset(result, "");

    do
    {
        memset(buffer, 0, s);

        if (fgets(buffer, s-1, stdin) == NULL)
        {
            fprintf(stdout, "Error reading input.\n");
            return;
        }

        minsock_stradd(result, buffer);
    }
    while (strstr(buffer, "\n") == NULL);
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

