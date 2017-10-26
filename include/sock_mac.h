#ifndef SOCK_MAC_H
#define SOCK_MAC_H

#include <sock_net.h>



class sock_mac : public sock_net {
public:
    sock_mac(int proto);
    sock_mac(int proto, int rec_index, int send_index=local_conf_valid-1);//the index in local
    void form_machd(my_mac *mac, char *src = NULL, char *des = NULL,  uint16_t type = htons(ETHERTYPE_ARP) );

    char * rid_mac(char *p, my_mac *mac=NULL);

    //set recv/send card index in local, from 0 to local_conf_valid-1
    int set_recv_card(int index=local_conf_valid-1);
    int set_send_card(int index=local_conf_valid-1);

    void show_mac(my_mac*p);
    virtual ~sock_mac();


    struct sockaddr_ll saddr, caddr;//send card:::recv card
protected:

private:

};

#endif // SOCK_MAC_H
