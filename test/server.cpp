#include <minsock.hpp>

int main(int argc, char **argv)
{
    minsock_win32_init();

    std::string request;
    std::string response;

    Minsock::Socket *server
        = new Minsock::Socket(
            "localhost", "123098",
            Minsock::LISTEN_SERVER);

    std::cout << "Server is running" << std::endl;

    Minsock::Socket *client = server->accept();

    std::cout << "Client Connected" << std::endl;

    // main event loop {{{
    while (1)
    {
        std::cout << "Waiting" << std::endl;

        request = client->recv();
        std::cout << "Incoming Message: '" << request << "'" << std::endl;

        if (request == "q\n")
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
