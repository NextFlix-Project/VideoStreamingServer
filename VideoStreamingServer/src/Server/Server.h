#pragma once
#include <crow.h>
 
namespace NextFlix
{
    class Server{
    public:
        Server() = delete;
        Server(uint16_t port);
        ~Server();
        
        crow::SimpleApp &getCrowApp();

    private:
        uint16_t port;
        crow::SimpleApp app;
        
        void advertiseServer();

    };
}