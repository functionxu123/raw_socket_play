#ifndef MAC_ALL_H
#define MAC_ALL_H

#include <sock_mac.h>
#include <mac_arp.h>


class mac_all : public sock_mac {
public:
    mac_all();
    mac_all(int ind);
    int recv_all(char *buf, int len=max_ether_len, int flag=0);
     int send_all(char *buf, int len, int flag=0);


    virtual ~mac_all();

protected:

private:

};

#endif // MAC_ALL_H
