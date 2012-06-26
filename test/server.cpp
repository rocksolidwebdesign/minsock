#include <minsock.hpp>

int main(int argc, char **argv)
{
    minsock_win32_init();

    Minsock::Socket *server;
    Minsock::Socket *client;

    std::string request;
    std::string response;

    server = new Minsock::Socket(
            "localhost", "123098",
            Minsock::LISTEN_SERVER);

    client = server->accept();

    std::cout << "Client Connected" << std::endl;

    // main event loop {{{
    while (1)
    {
        std::cout << "Waiting" << std::endl;

        request = client->recv();
        std::cout << "Incoming Message: '" << request << "'" << std::endl;

        if (request == "q")
        {
            std::cout << "Bye bye!" << std::endl;
            break;
        }

        response = "ACK\n";
        client->send(response);
    }
    // }}}

    delete server;
    delete client;

    minsock_win32_down();

    return 0;
}
