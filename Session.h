#ifndef SESSION_H
#define SESSION_H

#include <string>

#include "ARP_Packet.h"
#include "EthInterface.h"
#include "Sniffer.h"

class Session
{
public:

    Session(std::string if_name);
    Session(std::string if_name, std::string target_mac, std::string target_ip, std::string source_mac, std::string source_ip);


    ~Session();

    void start();

private:

    void sendResponse(struct arp_header* arpHeader);
    bool filterFrame(struct arp_header* arpReq);
    void printInterface();

    EthInterface* interface;
    Sniffer* sniffer;

    std::string target_ip;
    std::string target_mac;

    std::string sender_ip;
    std::string sender_mac;
};

#endif
