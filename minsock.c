#include <minsock.h>

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

    minsock_strset(&s->until, "\n");
    minsock_strset(&s->host, "");
    minsock_strset(&s->port, "");
    minsock_strset(&s->ip, "");
    minsock_strset(&s->response, "");
    minsock_strset(&s->request, "");

    *(s->connection) = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    minsock_err(*(s->connection), "Could not open the socket");

    return s;
}

void minsock_host(MINSOCKET *s, const char *host, const char *port)
{
    minsock_strset(&s->host, host);
    minsock_strset(&s->port, port);

    minsock_resolve(&s->ip, host);

    printf("Listening On %s:%s\n", s->ip, s->port);

    s->address->sin_family = AF_INET;

    /* set the host */
#ifdef WINDOWS
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
    int result;

    minsock_bind(s);

    result = listen(*(s->connection), 5);

    minsock_err(result, "Could not listen for connections");
}

MINSOCKET* minsock_accept(MINSOCKET *s)
{
    MINSOCKET *n = minsock_new();

    *(n->address_length) = sizeof(*(s->address));
    *(n->connection) = accept(*(s->connection), (struct sockaddr*)n->address, n->address_length);

    minsock_err(*(s->connection), "Could not accept socket connection");

    fprintf(stdout, "Established socket connection.\n");

    return n;
}

void minsock_recv(MINSOCKET *s, char **response)
{
    int z = 256;
    int status;
    char buffer[256];

    minsock_strset(response, "");

    do
    {
        memset(buffer, 0, z);
        status = recv(*(s->connection), buffer, z-1, 0);
        minsock_err(status, "Could not read socket");

        minsock_stradd(response, buffer);
    }
    while (strstr(buffer, s->until) == NULL);

    fprintf(stdout, "Socket read successful.\n");
}

void minsock_send(MINSOCKET *s, const char *request)
{
    int result;
    char *r = minsock_strnew();
    minsock_strset(&r, request);

    result = send(*(s->connection), r, strlen(r), 0);

    minsock_err(result, "Could not write socket");

    fprintf(stdout, "Socket write successful.\n");

    free(r);
}

void minsock_close(MINSOCKET *s)
{
#ifdef WINDOWS
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


void minsock_resolve(char **ip, const char *hostname)
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
        minsock_strset(ip, inet_ntoa(*addr_list[i]));
        return;
    }

    free(ip);
    free(host_info);
    free(*addr_list);
    free(addr_list);

    exit(1);
}

char * minsock_strnew()
{
    char *n = malloc(sizeof(char)+2);
    strcpy(n, "");
    return n;
}

void minsock_stradd(char **message, const char *append)
{
    *message = realloc(*message, strlen(*message)+strlen(append)+1);
    strcat(*message, append);
}

void minsock_strset(char **message, const char *source)
{
    *message = realloc(*message, strlen(source)+1);
    strcpy(*message, source);
}

void minsock_getline(char **result)
{
    int s = 256;
    char buffer[256];

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
#ifdef WINDOWS
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
#ifdef WINDOWS
    WSACleanup();
#endif
}

