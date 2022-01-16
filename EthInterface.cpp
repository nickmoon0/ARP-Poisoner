#include "EthInterface.h"

#include <arpa/inet.h>
#include <stdexcept>
#include <string.h>

/*
 * Constructor/Destructor
 */

EthInterface::EthInterface(const char* if_name)
{
    this->if_name = (char*)if_name;
    this->retrieveData();
}


/*
 * Methods
 */

void EthInterface::retrieveData()
{
    int fd;

    // Get interface name
    struct ifreq ifr;
    size_t if_name_len = strlen(if_name);

    // Copying interface name into ifreq struct
    if (if_name_len < sizeof(ifr.ifr_name))
    {
        memcpy(ifr.ifr_name, if_name, if_name_len);
        ifr.ifr_name[if_name_len] = 0;
    }
    else
    {
        throw std::runtime_error("Interface name is too long");
    }

    // Create socket to pass to ioctl
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }

    // Retrive interface index
    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0)
    {
        throw std::runtime_error("Failed to retrieve interface index");
    }
    this->if_index = ifr.ifr_ifindex;

    // Retrieve interface IP address
    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0)
    {
        throw std::runtime_error("Failed to retrieve interface IP");
    }

    struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;
    this->if_ip = inet_ntoa(ipaddr->sin_addr);

    // Retrieve MAC address
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0)
    {
        throw std::runtime_error("Failed to retrieve MAC address");
    }

    // Check if it is an ethernet interface
    if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER)
    {
        throw std::runtime_error("Interface is NOT an ethernet interface");
    }

    // Extract MAC address
    memcpy(if_mac, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
}

/*
 * Accessors
 */

char* EthInterface::get_if_name()
{
    return if_name;
}

char* EthInterface::get_if_ip()
{
    return if_ip;
}

int EthInterface::get_if_index()
{
    return if_index;
}

unsigned char* EthInterface::get_if_mac()
{
    return if_mac;
}
