#include "minsock.h"

int main(int argc, char* argv[])
{

    /* var declarations {{{ */
    char *reply,
         *response,
         *server_port,
         *server_host;

    minsock_fd *server_socket,
               *client_socket;

    minsock_addr *server_address,
                 *client_address;

    socklen_t client_address_len;
    /* }}} */
    /* memory allocation {{{ */
    reply          = malloc(sizeof(*reply));
    response       = malloc(sizeof(*response));
    server_host    = malloc(sizeof(*server_host));
    server_port    = malloc(sizeof(*server_port));
    server_socket  = malloc(sizeof(*server_socket));
    client_socket  = malloc(sizeof(*client_socket));
    server_address = malloc(sizeof(*server_address));
    client_address = malloc(sizeof(*client_address));

    client_address_len = sizeof(*client_address);
    /* }}} */
    /* init winsock {{{ */
    minsock_win32_init();
    /* }}} */
    /* config values {{{ */
    strcpy(server_host, "localhost");
    strcpy(server_port, "123098");
    /* }}} */
    /* listen for connection {{{ */
    minsock_mksocket(server_socket);
    minsock_mkaddr(server_address, server_host, server_port);
    minsock_bind(server_socket, server_address);
    minsock_listen(server_socket);
    minsock_accept(server_socket, client_socket, client_address, &client_address_len);

    printf("Client Connected\n");
    /* }}} */
    /* event loop {{{ */
    while (1)
    {
        strcpy(response, "");
        strcpy(reply, "");

        printf("Waiting\n");

        minsock_recv(client_socket, response, "\n");
        printf("Incoming Message: '%s'\n", response);

        if (strcmp(response, "q\n") == 0)
        {
            printf("Bye bye!\n");
            break;
        }

        strcpy(reply, "ACK\n");

        minsock_send(client_socket, reply);
    }
    /* }}} */
    /* cleanup {{{ */
    minsock_close(server_socket);
    minsock_close(client_socket);

    minsock_win32_down();

    free(response);
    free(reply);
    free(server_host);
    free(server_port);
    free(client_socket);
    free(server_socket);
    free(server_address);
    free(client_address);
    /* }}} */

    return 0;
}
