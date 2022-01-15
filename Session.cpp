#include "Session.h"

#include <cstring>
#include <iostream>
#include <stdio.h>

/*
 * Constructor
 */

Session::Session(std::string if_name)
{
    try
    {
        interface = new EthInterface(if_name.c_str());
        sniffer = new Sniffer(interface->get_if_index());

        // If interface and sniffer created successfully
        printInterface();
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
        throw e;
    }
}

/*
 * Methods
 */

// Start session
void Session::start()
{
    unsigned char frame[ARP_Packet::ARP_SIZE];

    bool sessionRunning = true;
    while (sessionRunning)
    {
        // Clear frame buffer and receive data
        memset(frame, 0, sizeof(frame));
        sniffer->receiveData(frame);

        // Create packet with received data and transmit response
		ARP_Packet ap(frame, interface->get_if_mac());
        sendResponse(ap);

    }
}

void Session::sendResponse(ARP_Packet packet)
{
    struct arp_header* header = packet.getArpHeader();
}

// Just to print the interface details for the user
void Session::printInterface()
{
    unsigned char* mac;

    // Use 2 spaces instead of tab. Looks neater this way
    std::cout << "Interface: " << interface->get_if_name() << std::endl;
    std::cout << "  index: " << interface->get_if_index() << std::endl;
    std::cout << "  ip: " << interface->get_if_ip() << std::endl;
    
    mac = interface->get_if_mac();
    std::cout << "  mac: ";
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}