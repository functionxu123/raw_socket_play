#ifndef MAC_ALL_H
#define MAC_ALL_H

#include <sock_mac.h>
#include <mac_arp.h>
typedef std::vector <nat_table>::iterator nat_ite;

class mac_all : public sock_mac {
public:
    mac_all();
    mac_all(int ind, int sen);
    int recv_all(char *buf, int len=max_ether_len, int flag=0);
     int send_all(char *buf, int len, int flag=0);
     int nat(int iner, int outer);//iner:which card is the inner net.    outer:the net link to the internet, both of them are the index in local
     int nat_get_port(my_ip *p, uint16_t stport=0);
     nat_ite nat_find(nat_table *p);
     nat_ite nat_find(uint16_t port);
     int checksum_ip_tcp(my_ip *p);
    virtual ~mac_all();

    std::vector <nat_table> table;
protected:

private:

};

#endif // MAC_ALL_H
