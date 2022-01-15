#include "ARP_Packet.h"

#include <arpa/inet.h>

#include <linux/if_ether.h>

#include <stdlib.h>
#include <string.h>

/*
 * Constructor/Destructor
 */

ARP_Packet::ARP_Packet(unsigned char* packet, unsigned char* local_mac)
{
    this->htype = htons(1);
    this->ptype = htons(ETH_P_IP);
    
    this->hlen = HARDWARE_LENGTH;
    this->plen = PROTOCOL_LENGTH;

    // 1 == arp request, 2 == arp reply
    this->opcode = htons(2);

    parseAddresses(packet, local_mac);
}

/*
 * Methods
 */

void ARP_Packet::parseAddresses(unsigned char* packet, unsigned char* local_mac)
{
    int sender_mac_start = 22;
    int target_mac_start = 32; // sender_mac_start + HARDWARE_LENGTH + PROTOCOL_LENGTH;

    // Copy local mac to sender mac
    memcpy(&sender_mac, local_mac, HARDWARE_LENGTH);
    
    // copy target ip to sender ip (Located immediately after target mac)
    memcpy(&sender_ip, &packet[target_mac_start + HARDWARE_LENGTH], PROTOCOL_LENGTH);

    // copy sender mac to target mac
    memcpy(&target_mac, &packet[sender_mac_start], HARDWARE_LENGTH);

    // Copy sender ip to target ip
    memcpy(&target_ip, &packet[sender_mac_start + HARDWARE_LENGTH], PROTOCOL_LENGTH);

    /*
    printf("Sender MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        sender_mac[0], sender_mac[1], sender_mac[2], sender_mac[3], sender_mac[4], sender_mac[5]);

    printf("Sender IP: %u.%u.%u.%u\n", sender_ip[0], sender_ip[1], sender_ip[2], sender_ip[3]);

    printf("Target MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        target_mac[0], target_mac[1], target_mac[2], target_mac[3], target_mac[4], target_mac[5]);

    printf("Target IP: %u.%u.%u.%u\n", target_ip[0], target_ip[1], target_ip[2], target_ip[3]);
    */
}

arp_header* ARP_Packet::getArpHeader()
{
    // If header has not yet been created
    if (this->header == nullptr)
    {
        this->header = (arp_header*)malloc(sizeof(arp_header));
        
        header->htype = this->htype;
        header->ptype = this->ptype;
        header->hlen = this->hlen;
        header->plen = this->plen;
        header->opcode = this->opcode;

        memcpy(header->sender_mac, this->sender_mac, HARDWARE_LENGTH);
        memcpy(header->sender_ip, this->sender_ip, PROTOCOL_LENGTH);

        memcpy(header->target_mac, this->target_mac, HARDWARE_LENGTH);
        memcpy(header->target_ip, this->target_ip, PROTOCOL_LENGTH);   
    }

    return this->header;
}