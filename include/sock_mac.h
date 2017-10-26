#ifndef SOCK_MAC_H
#define SOCK_MAC_H

#include <sock_net.h>



class sock_mac : public sock_net {
public:
    sock_mac(int proto);
    sock_mac(int proto, int index);
    void form_machd(my_mac *mac, char *src = NULL, char *des = NULL,  uint16_t type = htons(ETHERTYPE_ARP) );

    char * rid_mac(char *p, my_mac *mac=NULL);

    void show_mac(my_mac*p);
    virtual ~sock_mac();


    struct sockaddr_ll saddr;
protected:

private:

};

#endif // SOCK_MAC_H
