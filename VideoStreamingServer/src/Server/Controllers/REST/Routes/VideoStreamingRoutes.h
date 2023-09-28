#pragma once
#include <string>

namespace NextFlix
{
    class Server;
    class VideoStream;
    class VideoStreamingRoutes
    {
    private:
        Server *server;
        VideoStream *videoStream;

    public:
        VideoStreamingRoutes() = delete;
        VideoStreamingRoutes(Server *server);
        ~VideoStreamingRoutes();

    private:
        void initRoutes();
        void updateManifest(std::string id);

    };
}