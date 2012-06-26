#include <minsock.h>

int main(int argc, char **argv)
{
    minsock_win32_init();

    char *request  = minsock_strnew();
    char *response = minsock_strnew();

    MINSOCKET *server = minsock_start_server("localhost", "123098");
    printf("Server started. %d\n", *(server->connection));

    MINSOCKET *client = minsock_accept(server);
    printf("Client connected. %d\n", *(client->connection));

    /* main event loop {{{ */
    while (1)
    {
        printf("Waiting\n");

        minsock_strset(&response, "");
        minsock_recv(client, &response);

        printf("Incoming Message: '%s'\n", response);

        if (strcmp(response, "q\n") == 0)
        {
            printf("Bye bye!\n");
            break;
        }

        minsock_strset(&request, "ACK\n");
        minsock_send(client, request);
    }
    /* }}} */

    minsock_destroy(server);
    minsock_destroy(client);

    free(request);
    free(response);

    minsock_win32_down();

    return 0;
}
