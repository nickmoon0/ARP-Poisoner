#include "Sniffer.h"

#include <arpa/inet.h>
#include <cstring>

#include <iostream>

#include <linux/if_packet.h>

#include <net/ethernet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include "ARP_Packet.h"

/*
 * Constructors/Destructors
 */

Sniffer::Sniffer(int if_index)
{
    this->if_index = if_index;

    // Create raw ARP socket
    this->sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0)
    {
        // Failed to create socket
        throw std::runtime_error("Failed to create socket. Not run in sudo?");
    }
}

/*
 * Public methods
 */

void Sniffer::receiveData(unsigned char* frame)
{
    struct packet_mreq mreq = {0};
    mreq.mr_ifindex = if_index;
    mreq.mr_type = PACKET_MR_PROMISC;

    // ARP_SIZE + 1 so that we can check if frame is too large
    unsigned char frameBuffer[ARP_Packet::ARP_SIZE + 1];

    // Set socket options
    if (setsockopt(sock, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        throw std::runtime_error("Failed to set socket options");
    }

    struct sockaddr_ll src_addr;
    socklen_t src_addr_len = sizeof(src_addr);

    ssize_t count = recvfrom(sock, frameBuffer, sizeof(frameBuffer), 0, NULL, NULL);
    if (count < 0)
    {
        throw std::runtime_error("Failed to receive data");
    }
    else if (count == sizeof(frameBuffer))
    {
        // Frame was too large
        throw std::runtime_error("Read too much data into buffer. (Truncated)");
    }
    else
    {
        // Copy into frame
        memcpy(frame, frameBuffer, ARP_Packet::ARP_SIZE);
    }
}
