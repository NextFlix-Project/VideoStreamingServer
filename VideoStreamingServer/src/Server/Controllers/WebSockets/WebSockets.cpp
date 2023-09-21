#include "WebSockets.h"
#include "../../Server.h"
#include "../../../VideoStream.h"
#include <crow.h>
#include <Utilities/files.h>

#define CROW_ENFORCE_WS_SPEC

using namespace NextFlix;

WebSocket::WebSocket(Server *server)
{
    this->server = server;
    initWebSockets();
}

WebSocket::~WebSocket()
{
}

char *WebSocket::getBuffer()
{
    return this->buffer;
}
int WebSocket::initWebSockets()
{
    CROW_WEBSOCKET_ROUTE(server->getCrowApp(), "/upload<string>")
        .onopen([this](crow::websocket::connection &conn)
                { connections[conn.get_remote_ip()] = ""; })
        .onmessage([this](crow::websocket::connection &conn, const std::string &data, bool is_binary)
                   {
                       std::string id = connections[conn.get_remote_ip()];

                       if (is_binary)
                       {
                           if (!doesDirectoryExist("./movies/" + id))
                               createDirectory("./movies/" + id);

                           std::string filePath = "./movies/" + id + "/" + "filename";
                           std::ofstream outFile(filePath, std::ios::binary);
                           outFile.write(data.c_str(), data.size());
                           outFile.close();
                           std::cout << "File received: " << filePath << std::endl;
                       }
                       else
                       {
                           connections[conn.get_remote_ip()] = data;
                       }
                   })
        .onclose([this](crow::websocket::connection &conn, const std::string &reason)
                 { connections.erase(conn.get_remote_ip()); });

    return 0;
}
