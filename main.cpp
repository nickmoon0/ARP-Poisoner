#include <iostream>

#include "ARP_Packet.h"
#include "EthInterface.h"
#include "Sniffer.h"
#include "Session.h"

int main(int argc, char* argv[])
{
    std::string interface = "eth0";
    if (argc >= 2)
    {
        interface = argv[1];
        std::cout << "Using interface: " << interface << std::endl; 
    }
    else
    {
        std::cout << "No interface entered. Defaulting to " << interface << std::endl;
    }

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
