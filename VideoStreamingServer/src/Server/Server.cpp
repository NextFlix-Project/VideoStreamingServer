#include "Server.h"
#include "Controllers/REST/RestAPI.h"
#include "Controllers/WebSockets/WebSockets.h"
#include <curl/curl.h>

using namespace NextFlix;

Server::Server(uint16_t port){
 
    this->port = port;
    RestAPI *restAPI = new RestAPI(this);

    advertiseServer();

    uint16_t threadCount = 0;
    try
    {
        app.port(port).concurrency(threadCount).run();
    }
    catch (const std::exception &e)
    {
        std::cout << "REST server failed to run" << std::endl;
        std::cout << "Error: " << e.what() << std::endl;

    }
}

void Server::advertiseServer(){
        CURL* curl;
    CURLcode res;
    std::string response_data;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

     if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080/api/v1/server/internal/registerserver");
 
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: thisissomekeyforthevideoserverstocommunicate");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string serverPort = std::to_string(port);
        std::string serverType = "STREAMING";

        std::string json_data = R"({"port": ")" + serverPort + R"(", "serverType": ")" + serverType + R"("})";
    
        const char *postData = json_data.c_str();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl);


        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

Server::~Server()
{
    app.stop();
}

crow::SimpleApp &Server::getCrowApp()
{
    return app;
}