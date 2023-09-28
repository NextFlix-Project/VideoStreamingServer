#include "VideoStreamingRoutes.h"
#include <fstream>
#include "../../../../VideoStream.h"
#include <iostream>
#include "../../../Server.h"
#include <Utilities/files.h>
#include <Utilities/http.h>
#include <pugixml.hpp>
#include <filesystem>

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
        res.end(); });

    CROW_ROUTE(server->getCrowApp(), "/uploadfile")
        .methods(crow::HTTPMethod::Post)([this](const crow::request &req)
    {  
        std::string id = req.url_params.get("id");
                
        crow::multipart::message file_message(req);
        for (const auto& part : file_message.part_map)
        {
            if (!doesDirectoryExist("./movies/" + id ))
                createDirectory("./movies/" + id);

            std::string savePath = "./movies/" + id + "/";
            const auto& part_name = part.first;
            const auto& part_value = part.second;
            CROW_LOG_DEBUG << "Part: " << part_name;
            if ("file" == part_name)
            {
                // Extract the file name
                auto headers_it = part_value.headers.find("Content-Disposition");
                if (headers_it == part_value.headers.end())
                {
                    CROW_LOG_ERROR << "No Content-Disposition found";
                    return crow::response(400);
                }
                auto params_it = headers_it->second.params.find("filename");
                if (params_it == headers_it->second.params.end())
                {
                    CROW_LOG_ERROR << "Part with name \"InputFile\" should have a file";
                    return crow::response(400);
                }
                const std::string outfile_name = params_it->second;

                for (const auto& part_header : part_value.headers)
                {
                    const auto& part_header_name = part_header.first;
                    const auto& part_header_val = part_header.second;
                    CROW_LOG_DEBUG << "Header: " << part_header_name << '=' << part_header_val.value;
                    for (const auto& param : part_header_val.params)
                    {
                        const auto& param_key = param.first;
                        const auto& param_val = param.second;
                        CROW_LOG_DEBUG << " Param: " << param_key << ',' << param_val;
                    }
                }

                // Create a new file with the extracted file name and write file contents to it
                std::ofstream out_file(savePath + outfile_name);
                if (!out_file)
                {
                    CROW_LOG_ERROR << " Write to file failed\n";
                    continue;
                }
                out_file << part_value.body;
                out_file.close();
                CROW_LOG_INFO << " Contents written to " << outfile_name << '\n';
            }
            else
            {
                CROW_LOG_DEBUG << " Value: " << part_value.body << '\n';
            }
        }
        updateManifest(id);

        return crow::response(200);
    });
}

void VideoStreamingRoutes::updateManifest(std::string id)
{
    pugi::xml_document doc;

    if (!doc.load_file(("./output/" + id + "/manifest.mpd").c_str()))
    {
        std::cout << "Could not load manifest file." << std::endl;
        return;
    }

    pugi::xpath_node_set segmentTemplates = doc.select_nodes("//SegmentTemplate");

    std::string uri = "http://0.0.0.0:8888/stream/segment?id=" + id + "&" + "segment=";
    for (pugi::xpath_node segmentTemplateNode : segmentTemplates)
    {
        pugi::xml_node segmentTemplate = segmentTemplateNode.node();

        const char *initAttrib = segmentTemplate.attribute("initialization").value();

        segmentTemplate.attribute("initialization").set_value((uri + initAttrib).c_str());

        const char *mediaAttrib = segmentTemplate.attribute("media").value();
        segmentTemplate.attribute("media").set_value((uri + mediaAttrib).c_str());
    }

    if (!doc.save_file(("./output/" + id + "/manifest.mpd").c_str()))
    {
        std::cout << "Could not save XML file." << std::endl;
        return;
    }

    std::cout << "XML file successfully updated." << std::endl;
}