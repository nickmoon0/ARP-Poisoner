#ifndef ETHINTERFACE_H
#define ETHINTERFACE_H

#include <linux/if_ether.h>

#include <net/if.h>
#include <netinet/if_ether.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

class EthInterface
{
public:

	// Constructor
	EthInterface(const char* if_name);
	
	// Accessors
	char* get_if_name();
	char* get_if_ip();
	int get_if_index();
	unsigned char* get_if_mac();

private:

	// Variables
	char* if_name;
	char* if_ip;
	int if_index;
	unsigned char if_mac[ETHER_ADDR_LEN];

	// Methods
	void retrieveData();

};

#endif
