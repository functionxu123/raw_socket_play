#ifndef SOCK_MAC_H
#define SOCK_MAC_H

#include <sock_base.h>


class sock_mac : public sock_base {
public:
    sock_mac(int proto);
    void form_machd(my_mac *mac, char *src = NULL, char *des = NULL,  uint16_t type = htons(ETHERTYPE_ARP) );

    char * rid_mac(char *p, my_mac *mac);
    char *rid_ip(char *p, my_ip*ip);
    char *rid_tcp(char *p, my_tcp *tcp);
    virtual ~sock_mac();


    struct sockaddr_ll saddr;
protected:

private:

};

#endif // SOCK_MAC_H
