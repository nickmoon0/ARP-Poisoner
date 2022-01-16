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
    
    // Create structs
    createArpReq(packet);
    createArpHeader();

}

/*
 * Methods
 */

void ARP_Packet::createArpReq(unsigned char* packet)
{
    this->arpReq = (arp_header*)malloc(sizeof(arp_header));

    // Hardware and protocol type
    memcpy(&arpReq->htype, &packet[ETH_HEADER_LEN], 2);
    memcpy(&arpReq->ptype, &packet[ETH_HEADER_LEN + 2], 2);

    // Hardware and protocol length
    memcpy(&arpReq->hlen, &packet[ETH_HEADER_LEN + 4], 1);
    memcpy(&arpReq->plen, &packet[ETH_HEADER_LEN + 5], 1);

    // Operation type
    memcpy(&arpReq->opcode, &packet[ETH_HEADER_LEN + 6], 2);

    // Sender hardware and protocol address
    memcpy(arpReq->sender_mac, &packet[ETH_HEADER_LEN + 8], HARDWARE_LENGTH);
    memcpy(arpReq->sender_ip, &packet[ETH_HEADER_LEN + 14], PROTOCOL_LENGTH);

    // Target hardware and protocol address
    memcpy(arpReq->target_mac, &packet[ETH_HEADER_LEN + 18], HARDWARE_LENGTH);
    memcpy(arpReq->target_ip, &packet[ETH_HEADER_LEN + 24], PROTOCOL_LENGTH);
}

void ARP_Packet::createArpHeader()
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
}

arp_header* ARP_Packet::getArpHeader()
{
    return this->header;
}

arp_header* ARP_Packet::getArpReq()
{
    return this->arpReq;
}