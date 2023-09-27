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
    return 0;
}
