#include "ARP_Packet.h"

#include <arpa/inet.h>

#include <iostream>
#include <iomanip>

#include <linux/if_ether.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

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
    createArpRes();
}

ARP_Packet::~ARP_Packet()
{
    free(arpRes);
    free(arpReq);
}

/*
 * Methods
 */

void ARP_Packet::printArpHeader(struct arp_header* header)
{
    unsigned char highByte;
    unsigned char lowByte;

    // Change to print hex
    std::cout << std::hex;

    // Print hardware and protocol type
    highByte = header->htype >> 8;
    lowByte = header->htype & 0xFF;
    
    std::cout << "Hardware type: " << std::setw(2) << std::setfill('0') << +lowByte << " ";
    std::cout << std::setw(2) << std::setfill('0') << +highByte << std::endl;

    highByte = header->ptype >> 8;
    lowByte = header->ptype & 0xFF;

    std::cout << "Protocol type: " << std::setw(2) << std::setfill('0') << +lowByte << " ";
    std::cout << std::setw(2) << std::setfill('0') << +highByte << std::endl;

    // Print hardware and protocol length
    std::cout << "Hardware length: " << std::setw(2) << std::setfill('0') << +header->hlen << std::endl;
    std::cout << "Protocol length: " << std::setw(2) << std::setfill('0') << +header->plen << std::endl;

    // Print operation code
    highByte = header->opcode >> 8;
    lowByte = header->opcode & 0xFF;

    std::cout << "Operation code: " << std::setw(2) << std::setfill('0') << +lowByte << " ";
    std::cout << std::setw(2) << std::setfill('0') << +highByte << std::endl;

    // Print sender addresses
    std::cout << "Sender MAC: ";
    printAddress(header->sender_mac, HARDWARE_LENGTH);
    std::cout << "Sender IP: ";
    printAddress(header->sender_ip, PROTOCOL_LENGTH);

    std::cout << "Target MAC: ";
    printAddress(header->target_mac, HARDWARE_LENGTH);
    std::cout << "Target IP: ";
    printAddress(header->target_ip, PROTOCOL_LENGTH);

    // Revert back to printing decimal
    std::cout << std::dec;
}

void ARP_Packet::printAddress(u_int8_t address[], int addressSize)
{
    if (addressSize == HARDWARE_LENGTH)
    {
        printf("%02x:%02x:%02x:%02x:%02x:%02x\n", address[0], address[1], address[2], address[3], address[4], address[5]);
    }
    else if (addressSize == PROTOCOL_LENGTH)
    {
        printf("%u.%u.%u.%u\n", address[0], address[1], address[2], address[3]);
    }
    else
    {
        throw std::runtime_error("Unknown address length");
    }
}

void ARP_Packet::createArpReq(unsigned char* packet)
{
    this->arpReq = (struct arp_header*)malloc(sizeof(arp_header));

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

void ARP_Packet::createArpRes()
{
    this->arpRes = (struct arp_header*)malloc(sizeof(arp_header));
        
    arpRes->htype = this->htype;
    arpRes->ptype = this->ptype;
    arpRes->hlen = this->hlen;
    arpRes->plen = this->plen;
    arpRes->opcode = this->opcode;

    memcpy(arpRes->sender_mac, this->sender_mac, HARDWARE_LENGTH);
    memcpy(arpRes->sender_ip, this->sender_ip, PROTOCOL_LENGTH);

    memcpy(arpRes->target_mac, this->target_mac, HARDWARE_LENGTH);
    memcpy(arpRes->target_ip, this->target_ip, PROTOCOL_LENGTH); 
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

arp_header* ARP_Packet::getArpRes()
{
    return this->arpRes;
}

arp_header* ARP_Packet::getArpReq()
{
    return this->arpReq;
}