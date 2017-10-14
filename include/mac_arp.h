#ifndef MAC_ARP_H
#define MAC_ARP_H

#include <sock_mac.h>


class mac_arp : public sock_mac
{
    public:
        mac_arp();

        int get_arp_block(my_mac*mac, my_arp*arp);
        void form_arphd(char * ip_src,  char *ip_des, int op=1, char *mac_src=NULL, char * mac_des=NULL);

        virtual ~mac_arp();

    protected:

    private:
};

#endif // MAC_ARP_H
