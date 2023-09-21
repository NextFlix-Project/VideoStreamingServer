#include "RestAPI.h"
#include <stdio.h>
#include <fstream>
#include "../../Server.h"

using namespace NextFlix;

RestAPI::RestAPI(Server *server)
{
    this->server = server;
    this->initRestAPI();
}

RestAPI::~RestAPI()
{
    delete videoStreamingRoutes;
}

int RestAPI::initRestAPI()
{
    videoStreamingRoutes = new VideoStreamingRoutes(this->server);

    CROW_ROUTE(server->getCrowApp(), "/heartbeat")
    ([](const crow::request &, crow::response &res)
     {
        res.code = 200;
        res.end(); });

    return 0;
}
