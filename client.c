#include "minsock.h"

int main(int argc, char* argv[])
{

    /* var declarations {{{ */
    char *request,
         *response,
         *server_port,
         *server_host;

    minsock_fd   *server_socket;
    minsock_addr *server_address;

    socklen_t address_len;
    /* }}} */
    /* memory allocation {{{ */
    request            = malloc(sizeof(*request));
    response           = malloc(sizeof(*response));
    server_host        = malloc(sizeof(*server_host));
    server_port        = malloc(sizeof(*server_port));

    server_socket      = malloc(sizeof(*server_socket));
    server_address     = malloc(sizeof(*server_address));
    address_len        = sizeof(*server_address);
    /* }}} */
    /* init winsock {{{ */
    minsock_win32_init();
    /* }}} */
    /* config values {{{ */
    server_host  = "localhost";
    server_port  = "123098";
    /* }}} */
    /* connect to the server {{{ */
    minsock_mksocket(server_socket);
    minsock_mkaddr(server_address, server_host, server_port);
    minsock_connect(server_socket, server_address, &address_len);
    /* }}} */
    /* event loop {{{ */
    while (1)
    {
        strcpy(request,  "");
        strcpy(response, "");

        printf("Send a message: ");
        minsock_get_until(request, "\n");

        minsock_send(server_socket, request);

        if (strcmp(request, "q\n") == 0)
        {
            printf("Bye bye!\n");
            break;
        }

        printf("You sent: '%s'\n", request);
        printf("\n");

        printf("Listening for response\n");
        minsock_recv(server_socket, response, "\n");

        printf("Handshake Message: '%s'\n", response);
    }
    /* }}} */
    /* cleanup {{{ */
    minsock_close(server_socket);

    minsock_win32_down();

    free(request);
    free(response);
    free(server_host);
    free(server_port);
    free(server_socket);
    free(server_address);
    /* }}} */

    return 0;
}
