#include "VideoStreamingRoutes.h"
#include <fstream>
#include <string>
#include "../../../../VideoStream.h"
#include <iostream>
#include "../../../Server.h"
#include <Utilities/files.h>
#include <Utilities/http.h>

using namespace NextFlix;

VideoStreamingRoutes::VideoStreamingRoutes(Server *server)
{
    this->server = server;

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
        std::string id = req.url_params.get("id");

        std::ifstream mpdFile("./movies/" + id + "/manifest.mpd");

        if (!mpdFile.is_open())
            res.body = "Error";
        else{
             std::string fileContents((std::istreambuf_iterator<char>(mpdFile)),
                              std::istreambuf_iterator<char>());
                    
            res.add_header("Access-Control-Allow-Origin", "*");             
            res.add_header("Content-Type", "application/dash+xml");
            res.add_header("Content-Length", std::to_string(fileContents.size()));   
            res.write(fileContents);
        }
    
        res.end(); });

    CROW_ROUTE(server->getCrowApp(), "/stream/segment")
    ([](const crow::request &req, crow::response &res)
     {
        std::string id = req.url_params.get("id");
        std::string segment = req.url_params.get("segment");
        try {
 
            std::string segmentFilePath = "./movies/" + id + "/" + segment;

            res.add_header("Access-Control-Allow-Origin", "*");
            res.set_header("Content-Type", "video/iso.segment");
            res.set_static_file_info(segmentFilePath);

        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
        res.end(); 
    });

    CROW_ROUTE(server->getCrowApp(), "/uploadfile").methods("POST"_method)([this](const crow::request &req, crow::response &res)
    {
        std::string id = req.url_params.get("id");

        if (!isMultipartFormData(req.get_header_value("Content-Type")))
        {
            res.code = 400;
            res.write("Invalid request");
            res.end();
            return;
        }
        
        size_t boundaryPos = 0;

        std::string boundary = getBoundaryFromHeader(req.get_header_value("Content-Type"), boundaryPos);
        std::vector<std::string> parts;
        std::string body = req.body;
        
        while (boundaryPos != std::string::npos) 
        {      
            size_t nextBoundaryPos = body.find(boundary, boundaryPos + boundary.size());
        
            if (nextBoundaryPos != std::string::npos)
            {
                std::string part = body.substr(boundaryPos + boundary.size(), nextBoundaryPos - (boundaryPos + boundary.size()));
                parts.push_back(part);
            }
            boundaryPos = nextBoundaryPos;
        }
        
        std::string outputFile = "";
        std::string inputFile = "";

        for (const auto& part : parts) 
        {
            size_t filenamePos = part.find("filename=\"");
        
            if (filenamePos != std::string::npos)
            {
                size_t filenameEndPos = part.find("\"", filenamePos + 10);
                
                if (filenameEndPos != std::string::npos)
                {
                    std::string filename = part.substr(filenamePos + 10, filenameEndPos - (filenamePos + 10));
                    size_t contentPos = part.find("\r\n\r\n") + 4;
                    std::string content = part.substr(contentPos);
                    
                    if (!doesDirectoryExist("./movies/" + id ))
                        createDirectory("./movies/" + id);

                    std::string savePath = "./movies/" + id + "/" + filename;
                    inputFile = savePath;
                    
                    std::ofstream outfile(savePath, std::ofstream::binary);
                    outfile.write(content.data(), content.size());
                    outfile.close();
                }
            }
        }
        res.write("File received successfully");
        res.end(); 
    });
}