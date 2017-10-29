#ifndef MAC_ALL_H
#define MAC_ALL_H

#include <sock_mac.h>
#include <mac_arp.h>
class nat_table{
public:
    uint32_t ip;
    int iner_port;
    int out_port;
    bool operator<(const nat_table&t) const{
        return out_port<t.out_port;
    }
};


typedef std::set <nat_table>::iterator nat_ite;

class mac_all : public sock_mac {
public:
    mac_all();
    mac_all(int ind, int sen);
    int recv_all(char *buf, int len=max_ether_len, int flag=0);
     int send_all(char *buf, int len, int flag=0);
     int nat(uint32_t st, uint32_t ed, int outer);//iner:which card is the inner net.    outer:the net link to the internet, both of them are the index in local
     int nat_get_port(my_ip *p, nat_table *t);
     nat_ite nat_find(nat_table *p);
     nat_ite nat_find(uint16_t port);
     int nat_toiner(char *, nat_ite p, mac_arp&arp);
     int nat_toouter(char *, int, mac_arp & arp);
     uint16_t nat_set_sport(my_ip*p, uint16_t port);
     uint16_t nat_set_dport(my_ip *p, uint16_t port);
     int checksum_ip_tcp(my_ip *p);
    virtual ~mac_all();

    std::set <nat_table> table;
protected:

private:

};

#endif // MAC_ALL_H
