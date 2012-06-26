#include "minsock.hpp"

namespace Minsock
{
    Socket::Socket()
    {
        this->s = minsock_new();
    }

    Socket::Socket(MINSOCKET *s)
    {
        this->s = s;
    }

    Socket::Socket(std::string host, std::string port)
    {
        this->s = minsock_init(host.c_str(), port.c_str());
    }

    Socket::Socket(std::string host, std::string port, int type)
    {
        switch (type) {
            case LISTEN_SERVER:
                this->s = minsock_start_server(host.c_str(), port.c_str());
                break;
            case CONNECT_CLIENT:
                this->s = minsock_open_connection(host.c_str(), port.c_str());
                break;
        }
    }

    Socket::~Socket()
    {
        minsock_free(this->s);
    }

    void Socket::connect()
    {
        minsock_connect(this->s);
    }

    void Socket::connect(std::string host, std::string port)
    {
        minsock_host(this->s, host.c_str(), port.c_str());
        minsock_connect(this->s);
    }

    void Socket::listen(std::string host, std::string port)
    {
    }

    Socket* Socket::accept(void)
    {
        MINSOCKET *n;
        n = minsock_accept(this->s);

        Socket *s;
        s = new Socket(n);

        return s;
    }

    void Socket::send(std::string message)
    {
        strcpy(this->s->request, message.c_str());
        minsock_send(this->s);
    }

    std::string Socket::recv()
    {
        minsock_recv(this->s);

        std::string response(this->s->response);

        return response;
    }

    void Socket::close(void)
    {
        minsock_close(this->s);
    }
}
