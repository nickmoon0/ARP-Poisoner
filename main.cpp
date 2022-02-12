#include <iostream>
#include <unistd.h>

#include "Session.h"

void printOptions()
{
    std::cout << "-t <MAC address>" << std::endl;
    std::cout << "\t" << "Allows user to specify target MAC Address." << std::endl;

    std::cout << std::endl;

    std::cout << "-f <IP address>" << std::endl;
    std::cout << "\t" << "Allows user to specify target IP. This will only respond to ARP requests looking for this IP addresss" << std::endl;

    std::cout << std::endl;

    std::cout << "-m <MAC address>" << std::endl;
    std::cout << "\t" << "Allows user to specify sender MAC. This will only respond to ARP requests originating from this MAC addresss" << std::endl;

    std::cout << std::endl;

    std::cout << "-s <IP address>" << std::endl;
    std::cout << "\t" << "Allows user to specify sender IP. This will only respond to ARP requests originating from this IP address" << std::endl;

    std::cout << std::endl;

    std::cout << "-i <interface name>" << std::endl;
    std::cout << "\t" << "Allows user to specify interface. This will bind the socket to the specified interface (so long as interface exists)" << std::endl;

    std::cout << std::endl;

    std::cout << "-h" << std::endl;
    std::cout << "\t" << "Displays this menu" << std::endl;
}

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
            while ((opt = getopt(argc, argv, ":t:f:m:s:i:h")) != -1)
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
                    case 's':
                        sender_ip = optarg;
                    break;

                    // Interface
                    case 'i':
                        interface = optarg;
                    break;

                    // Help
                    case 'h':
                        printOptions();
                        return 0; // Dont want to continue execution after help printeds
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
        Session s(interface, target_mac, target_ip, sender_mac, sender_ip);
        s.start();
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
    }   
    
    return 0;
}
