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
    ~Session();

    void start();

private:

    void sendResponse(struct arp_header* arpHeader);
    void printInterface();

    EthInterface* interface;
    Sniffer* sniffer;
};

#endif
