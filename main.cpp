#include <iostream>

#include "ARP_Packet.h"
#include "EthInterface.h"
#include "Sniffer.h"
#include "Session.h"

int main(int argc, char* argv[])
{
	std::string interface = "eth0";
	if (argc >= 2)
	{
		interface = argv[1];
		std::cout << "Using interface: " << interface << std::endl; 
	}
	else
	{
		std::cout << "No interface entered. Defaulting to " << interface << std::endl;
	}
	Session s(interface);
	s.start();
	return 0;
}
