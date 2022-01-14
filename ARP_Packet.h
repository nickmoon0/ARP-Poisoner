#ifndef ARP_PACKET_H
#define ARP_PACKET_H

#include <sys/types.h>

class ARP_Packet
{
public:

	// Size in bytes
	const static int ARP_SIZE = 64;
	const static int HARDWARE_LENGTH = 6;
	const static int PROTOCOL_LENGTH = 4;
	

private:

};

// ARP header used to send requests
struct arp_header
{
	u_int16_t htype;
	u_int16_t ptype;
	u_int8_t hlen;
	u_int8_t plen;
	u_int16_t opcode;
	u_int8_t sender_mac[ARP_Packet::HARDWARE_LENGTH];
	u_int8_t sender_ip[ARP_Packet::PROTOCOL_LENGTH];
	u_int8_t target_mac[ARP_Packet::HARDWARE_LENGTH];
	u_int8_t target_ip[ARP_Packet::PROTOCOL_LENGTH];
} typedef arp_header;


#endif
