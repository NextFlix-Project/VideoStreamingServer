#include <iostream>
#include <fstream>
#include <string>
#include "Server/Server.h"

#define DEBUG 1

int main(int argc, char *argv[])
{
    uint port = 0;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (i + 1 < argc)
        {
            if (arg == "-port" && i + 1 < argc)
            {
                port = std::stoi(argv[i + 1]);

                if (port < 0 || port > 65535)
                {
                    std::cout << "Invalid port..." << std::endl;
                    return 1;
                }
            }
        }
    }

#if DEBUG
    port = 8888;
#endif

    NextFlix::Server *server = new NextFlix::Server(port);

    return 0;
}
