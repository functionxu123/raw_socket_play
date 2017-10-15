#ifndef MAC_ARP_H
#define MAC_ARP_H

#include <sock_mac.h>


class mac_arp : public sock_mac
{
    public:
        mac_arp();

        void form_arp(my_arp *arp, char * ip_src,  char *ip_des, int op=1, char *mac_src=NULL, char * mac_des=NULL);

        int send_arp(my_mac * mac, my_arp*arp, int flag=0);
        int recv_arp(my_mac * mac, my_arp*arp, int flag=0);

        void show_mac(my_mac*p);
        void show_arp(my_arp *p);
        virtual ~mac_arp();

    protected:

    private:
};

#endif // MAC_ARP_H
