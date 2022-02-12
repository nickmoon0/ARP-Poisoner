#include <iostream>
#include <unistd.h>

#include "Session.h"

int main(int argc, char* argv[])
{
    std::string interface = "eth0";

    std::string target_ip = "";
    std::string target_mac = "";
    
    std::string sender_ip = "";
    std::string sender_mac = "";

    if (argc <= 1)
    {
        std::cout << "No parameters entered. Using default settings." << std::endl;
    }
    else // parse arguments
    {
        bool isCaseInsensitive = false;
        int opt;

        // Try/catch for any incorrectly entered arguments
        try
        {
            while ((opt = getopt(argc, argv, ":t:f:m:h:i:")) != -1)
            {
                switch(opt)
                {
                    // Target MAC
                    case 't':
                        target_mac = optarg;
                    break;
                    // Target IP
                    case 'f':
                        target_ip = optarg;
                    break;

                    // Sender MAC
                    case 'm':
                        sender_mac = optarg;
                    break;

                    // Sender IP
                    case 'h':
                        sender_ip = optarg;
                    break;

                    // Interface
                    case 'i':
                        interface = optarg;
                    break;

                    // Incorrect options
                    case ':':
                        throw std::runtime_error("No value entered after option");
                    break;
                    case '?':
                        throw std::runtime_error("Unknown option entered");
                    break;
                }
            }
        }
        catch (std::runtime_error e)
        {
            std::cout << e.what() << std::endl;
            return -1;
        }

    }

    // Begin session
    try
    {
        Session s(interface);
        s.start();
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
    }   
    
    return 0;
}
