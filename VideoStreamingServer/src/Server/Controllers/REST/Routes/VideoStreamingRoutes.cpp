#include "VideoStreamingRoutes.h"
#include <fstream>
#include <string>
#include "../../../../VideoStream.h"
#include <iostream>
#include "../../../Server.h"

using namespace NextFlix;

VideoStreamingRoutes::VideoStreamingRoutes(Server *server)
{
    this->server = server;

    // std::ifstream *file = new std::ifstream("./video_dashinit.mp4", std::ios::in | std::ios::binary);

    //   if (!file->is_open())
    //    std::cout << "File not open" << std::endl;

    //  videoStream = new NextFlix::VideoStream(0000000, file, 0);

    this->initRoutes();
}

VideoStreamingRoutes::~VideoStreamingRoutes()
{
    delete this->videoStream;
}

void VideoStreamingRoutes::initRoutes()
{
    CROW_ROUTE(server->getCrowApp(), "/stream")
    ([this](const crow::request &req, crow::response &res)
     {
        std::ifstream mpdFile("./vids/output.mpd");

        if (!mpdFile.is_open())
            res.body = "Error";
        else{
             std::string fileContents((std::istreambuf_iterator<char>(mpdFile)),
                              std::istreambuf_iterator<char>());
                    
            res.add_header("Access-Control-Allow-Origin", "*");             
            res.add_header("Content-Type", "application/dash+xml");
            res.add_header("Content-Length", std::to_string(fileContents.size())); // Set Content-Length

            res.write(fileContents);

        }
    
        res.end(); 
    });

    CROW_ROUTE(server->getCrowApp(), "/streams/segment/<string>")
([](const crow::request& req, crow::response& res, std::string segment) {
     std::string segmentFilePath = "./vids/" + segment;

                 res.add_header("Access-Control-Allow-Origin", "*");             

        res.set_header("Content-Type", "video/iso.segment");

     res.set_static_file_info(segmentFilePath);
    
   
    
    res.end();  

});


    
}