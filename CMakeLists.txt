cmake_minimum_required(VERSION 3.10)

# Set project name
project(arp_spoofer)

# Add compile options
add_compile_options(-g)

# Add executable
add_executable(arp_spoofer main.cpp Session.cpp ARP_Packet.cpp Sniffer.cpp EthInterface.cpp)

# Set standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
