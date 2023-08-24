#pragma once

#include "Routes/VideoStreamingRoutes.h"

namespace NextFlix
{
    class Server;

    class RestAPI
    {
        public:
            RestAPI() = delete;
            RestAPI(Server *server);
            ~RestAPI();

        private: 
            int threadCount;
            VideoStreamingRoutes *videoStreamingRoutes;
            Server *server;
            
        private:
            int initRestAPI();
    };
}