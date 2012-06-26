#include <minsock.h>

int main(int argc, char **argv)
{
    minsock_win32_init();

    MINSOCKET *server;
    MINSOCKET *client;

    server = minsock_start_server("localhost", "123098");
    printf("Server started. %d\n", *(server->connection));

    client = minsock_accept(server);

    printf("Client connected. %d\n", *(client->connection));

    /* main event loop {{{ */
    while (1)
    {
        printf("Waiting\n");

        strcpy(client->response, "");
        minsock_recv(client);

        printf("Incoming Message: '%s'\n", client->response);

        if (strcmp(client->response, "q\n") == 0)
        {
            printf("Bye bye!\n");
            break;
        }

        strcpy(client->request, "ACK\n");
        minsock_send(client);
    }
    /* }}} */

    minsock_destroy(server);
    minsock_destroy(client);

    minsock_win32_down();

    return 0;
}
