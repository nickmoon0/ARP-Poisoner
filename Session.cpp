#include "Session.h"

#include <arpa/inet.h>

#include <cstring>
#include <iostream>

#include <linux/if_packet.h>

#include <net/ethernet.h>
#include <netinet/ip.h>

#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

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

        try
        {
            sendResponse(ap);

            std::cout << "Sent response:" << std::endl;

        }
        catch (std::runtime_error e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }   
    
    }
}

void Session::sendResponse(ARP_Packet packet)
{
    // create an ethernet header big enough to encapsulate arp response
    int frameSize = 2 * HARDWARE_LENGTH + 2 + sizeof(arp_header);
    int arpSize = 28;
    u_int8_t ethHeader[frameSize];
    
    struct arp_header* arpHeader;
    struct sockaddr_ll address;

    int sock;
    int bytes;

    // Get arp header and use it to create ethernet header
    arpHeader = packet.getArpHeader();
    
    memcpy(ethHeader, arpHeader->target_mac, HARDWARE_LENGTH * sizeof(u_int8_t));
    memcpy(ethHeader + HARDWARE_LENGTH, arpHeader->sender_mac, HARDWARE_LENGTH * sizeof(u_int8_t));
    ethHeader[2 * HARDWARE_LENGTH] = ETH_P_ARP / 256;
    ethHeader[2 * HARDWARE_LENGTH + 1] = ETH_P_ARP % 256;

    // Encapsulate ARP header
    memcpy(&ethHeader[ETH_HEADER_LEN], arpHeader, sizeof(arp_header));

    // Fill out address struct (sockaddr_ll)
    address.sll_family = AF_PACKET;
    address.sll_ifindex = interface->get_if_index();
    address.sll_halen = htons(HARDWARE_LENGTH);
    memcpy(address.sll_addr, arpHeader->sender_mac, HARDWARE_LENGTH * sizeof(u_int8_t));

    // Create socket to send data
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }

    bytes = sendto(sock, ethHeader, frameSize, 0, (struct sockaddr*)&address, sizeof(address));
    if (bytes <= 0)
    {
        throw std::runtime_error("Failed to send response");
    }

    close(sock);
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