# ARP-Spoofer

A simple ARP spoofer. Uses C sockets library.

## How to use

```bash
cd ARP-Spoofer
cmake .
cmake --build .
sudo ./arp_spoofer [options]
```
> Use -h for a list of options and their descriptions
<br>
Note that it has to be run as sudo (in order to create socket). The interface will default to eth0.


