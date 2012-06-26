#include <minsock.h>

int main(int argc, char **argv)
{
    minsock_win32_init();

    MINSOCKET *server;

    server = minsock_open_connection("localhost", "123098");

    fprintf(stdout, "Connected to server. %d\n", *(server->connection));

    /* main event loop {{{ */
    while (1)
    {
        fprintf(stdout, "Send a message: ");
        minsock_getline(server->request);
        fprintf(stdout, "Sending: '%s'\n", server->request);

        minsock_send(server);

        if (strcmp(server->request, "q\n") == 0)
        {
            printf("Bye bye!\n");
            break;
        }

        fprintf(stdout, "You sent: '%s'\n", server->request);
        fprintf(stdout, "\n");

        fprintf(stdout, "Listening for response\n");
        minsock_recv(server);

        fprintf(stdout, "Handshake Message: '%s'\n", server->response);
    }
    /* }}} */

    minsock_destroy(server);

    minsock_win32_down();

    return 0;
}
