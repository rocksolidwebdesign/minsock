#include <iostream>
#include <vector>
#include <string>

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

extern "C" {
    #include "minsock.h"
}

namespace Minsock
{
    enum { LISTEN_SERVER, CONNECT_CLIENT };

    class DLLIFACE Socket
    {
        MINSOCKET *s;

        public:
            Socket();
            Socket(MINSOCKET *s);
            Socket(std::string host, std::string port);
            Socket(std::string host, std::string port, int type);
            ~Socket();
            void connect();
            void connect(std::string host, std::string port);
            void listen(std::string host, std::string port);
            Socket* accept(void);
            void send(std::string message);
            std::string recv(void);
            void close(void);
    };
}
