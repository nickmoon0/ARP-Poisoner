#ifndef SNIFFER_H
#define SNIFFER_H

class Sniffer
{
public:
    
    Sniffer(int if_index);
    void receiveData(unsigned char* frame);

private:

    int sock;
    int if_index;

};

#endif
