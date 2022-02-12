#ifndef SESSION_H
#define SESSION_H

#include <string>

#include "ARP_Packet.h"
#include "EthInterface.h"
#include "Sniffer.h"

class Session
{
public:
    Session(std::string if_name, std::string target_mac, std::string target_ip, std::string sender_mac, std::string sender_ip);

    ~Session();

    void start();

private:

    void sendResponse(struct arp_header* arpHeader);
    bool filterFrame(struct arp_header* arpReq);
    void printInterface();

    char* convertIP(unsigned char* ip);
    char* convertMAC(unsigned char* mac);

    EthInterface* interface;
    Sniffer* sniffer;

    std::string target_ip;
    std::string target_mac;

    std::string sender_ip;
    std::string sender_mac;
};

#endif
