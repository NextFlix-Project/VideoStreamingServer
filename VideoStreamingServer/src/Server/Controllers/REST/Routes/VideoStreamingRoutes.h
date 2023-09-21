#pragma once

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
    };
}