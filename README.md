# ARP-Spoofer

A simple ARP spoofer. Uses C sockets library.

## How to use

```bash
cd ARP-Spoofer
cmake .
cmake --build .
sudo ./arp_spoofer [optional interface]
```

Note that it has to be run as sudo. The interface will default to eth0.
