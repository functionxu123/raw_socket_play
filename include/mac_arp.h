#ifndef MAC_ARP_H
#define MAC_ARP_H

#include <sock_mac.h>
#include <vector>
#include <algorithm>


class mac_arp : public sock_mac
{
    public:
        mac_arp();

        void form_arp(my_arp *arp, char * ip_src,  char *ip_des, int op=1, char *mac_src=NULL, char * mac_des=NULL);
        void form_arp(my_arp *arp, uint32_t ip_src,  uint32_t ip_des, int op=1, char *mac_src=NULL, char * mac_des=NULL);

        int send_arp(my_mac * mac, my_arp*arp, int flag=0);
        int recv_arp(my_mac * mac, my_arp*arp, int flag=0);

        int arp_cheat_send(char *start=NULL, char *endsss=NULL);
        int arp_cheat_send(uint32_t start, uint32_t endd);//net sequence
        int arp_cheat_send(uint32_t ip);//net sequence

        uint32_t arp_cheat_recv(char *start=NULL, char *endsss=NULL);
        uint32_t arp_cheat_recv(uint32_t start, uint32_t endd);//net sequence
        uint32_t arp_cheat_recv(uint32_t ip);//net sequence

        uint32_t arp_cheat(char *start=NULL, char *endsss=NULL);
        uint32_t arp_cheat(uint32_t start, uint32_t endd);//net sequence
        uint32_t arp_cheat(uint32_t ip);//net sequence

        int scan_ip_arp(uint32_t st, uint32_t ed, std::vector<uint32_t> &vec);//net seq
        int scan_ip_arp(std::vector<uint32_t> &vec, char * st=NULL, char * ed=NULL);//net seq

        void show_arp(my_arp *p);
        virtual ~mac_arp();

    protected:

    private:
};

#endif // MAC_ARP_H
