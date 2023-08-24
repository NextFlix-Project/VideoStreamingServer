#include "WebSockets.h"
#include "../../Server.h"
#include "../../../VideoStream.h"
#include <crow.h>
 

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
    file = new std::ifstream("./video.mp4", std::ios::in | std::ios::binary);

    if (!file->is_open())
        std::cout << "File not open" << std::endl;

    int length = 1024 * 1024;
   


    // allocate memory:
    if (buffer != NULL) delete buffer;
    this->buffer = new char [length];
 
  //  file->read (buffer,length);
   std::cout <<   file->gcount() << std::endl; 
    //file->close();
  //  VideoStream *videoStream = new NextFlix::VideoStream(0000000, file, 0);

    CROW_WEBSOCKET_ROUTE(server->getCrowApp(), "/ws")
    .onopen([this](crow::websocket::connection& conn){
                // Create stream
                // Add Stream
                  //     char * bindata = videoStream->getBufferNext();
                
                  char * bindata =new char [1024 * 1024];
         this->file->read (bindata, 1024 * 1024);

        
                conn.send_binary(crow::utility::base64encode(reinterpret_cast<const unsigned char*>(bindata), sizeof(bindata)));
                             std::cout << "Connected" << std::endl;
            })
    .onclose([this](crow::websocket::connection& conn, const std::string& reason){
                std::cout << reason << std::endl;
            })
    .onmessage([this](crow::websocket::connection& conn, const std::string& data, bool is_binary){
                if (is_binary)
                std::cout << data << std::endl;
                else
                std::cout << data << std::endl;

        
                // char * bindata = videoStream->getBufferNext();
           
                     char * bindata = this->getBuffer();
         
                conn.send_binary(bindata);
            }); 

    return 0;
}

void WebSocket::addStream(VideoStream *stream)
{
    openStreams[stream->getUserUUID()] = stream;
}

int WebSocket::removeStream(uint64_t uuid)
{
    if (openStreams.find(uuid) != openStreams.end())
    {
        openStreams.erase(uuid);
        return 0;
    }

    return 1;
}