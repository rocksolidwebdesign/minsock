#include <minsock.h>

int main(int argc, char **argv)
{
    minsock_win32_init();

    char *request  = minsock_strnew();
    char *response = minsock_strnew();

    MINSOCKET *server = minsock_open_connection("localhost", "123098");

    fprintf(stdout, "Connected to server. %d\n", *(server->connection));

    /* main event loop {{{ */
    while (1)
    {
        fprintf(stdout, "Send a message: ");
        minsock_getline(&request);
        fprintf(stdout, "Sending: '%s'\n", request);

        minsock_send(server, request);

        if (strcmp(request, "q\n") == 0)
        {
            printf("Bye bye!\n");
            break;
        }

        fprintf(stdout, "You sent: '%s'\n", request);
        fprintf(stdout, "\n");

        fprintf(stdout, "Listening for response\n");
        minsock_recv(server, &response);

        fprintf(stdout, "Handshake Message: '%s'\n", response);
    }
    /* }}} */

    minsock_destroy(server);

    free(request);
    free(response);

    minsock_win32_down();

    return 0;
}
