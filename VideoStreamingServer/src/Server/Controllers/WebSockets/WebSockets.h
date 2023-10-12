#pragma once
#include <fstream>
#include <unordered_map>
#include <stdint.h>
#include <map>
#include <string>
namespace NextFlix
{
    class Server;
    class VideoStream;

    class WebSocket
    {
    public:
        WebSocket() = delete;
        WebSocket(Server *server);
        ~WebSocket();

    private:
        Server *server;
        char *buffer;
        std::unordered_map<uint64_t, VideoStream *> openStreams;
        std::ifstream *file;
        std::map<std::string, std::string> connections;

    private:
        int initWebSockets();
        void addStream(VideoStream *videoStream);
        int removeStream(uint64_t uuid);
        char *getBuffer();
        };
}