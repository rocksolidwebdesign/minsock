#include <minsock.hpp>

int main(int argc, char **argv)
{
    minsock_win32_init();

    std::string request;
    std::string response;

    Minsock::Socket *server;

    server = new Minsock::Socket(
            "localhost", "123098",
            Minsock::CONNECT_CLIENT);

    std::cout << "Connected to server" << std::endl;

    // main event loop {{{
    while (1)
    {
        std::cout << "Send a message: ";

        std::cin >> request;
        std::cout << "You entered: '" << request << "'" << std::endl;

        server->send(request);

        if (request == "q\n")
        {
            std::cout << "Bye Bye!" << std::endl;
            break;
        }

        std::cout << "You sent: '" << request << "'" << std::endl;
        std::cout << "Listening for response." << std::endl;

        response = server->recv();

        std::cout << "Handshake Message: '" << response << "'" << std::endl;
    }
    // }}}

    delete server;

    minsock_win32_down();

    return 0;
}
