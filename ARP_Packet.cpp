#include "ARP_Packet.h"

#include <arpa/inet.h>

#include <linux/if_ether.h>

#include <string.h>

#include <iostream>
/*
 * Constructor/Destructor
 */

ARP_Packet::ARP_Packet(unsigned char* packet, int opcode)
{
    this->htype = htons(1);
    this->ptype = htons(ETH_P_IP);
    
    this->hlen = HARDWARE_LENGTH;
    this->plen = PROTOCOL_LENGTH;

    this->opcode = htons(opcode);
    this->int_opcode = opcode;

    parseAddresses(packet);
}

/*
 * Methods
 */

void ARP_Packet::parseAddresses(unsigned char* packet)
{
    // 1 == request, 2 == reply
    if (int_opcode == 2)
    {
        
    }
    else
    {
        
    }
}