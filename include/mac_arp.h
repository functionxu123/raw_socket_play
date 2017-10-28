#ifndef MAC_ARP_H
#define MAC_ARP_H

#include <sock_mac.h>
#include <vector>
#include <algorithm>
#include <functional>


class mac_arp : public sock_mac
{
    public:
        mac_arp();
        mac_arp(int ind, int send_card);//choose the netcard order, from 0 to local_conf_valid-1, default is local_conf_valid-1

        void form_arp(my_arp *arp, char * ip_src,  char *ip_des, int op=1, char *mac_src=NULL, char * mac_des=NULL);
        void form_arp(my_arp *arp, uint32_t ip_src,  uint32_t ip_des, int op=1, char *mac_src=NULL, char * mac_des=NULL);

        int send_arp(my_mac * mac, my_arp*arp, int flag=0);
        int recv_arp(my_mac * mac, my_arp*arp, int flag=0);

        int arp_cheat_send(char *start=NULL, char *endsss=NULL);
        int arp_cheat_send(uint32_t start, uint32_t endd);//net sequence
        int arp_cheat_send(uint32_t ip, char *mac=NULL);//net sequence

        int arp_cheat_recv(char *start=NULL, char *endsss=NULL,tar_info *p=NULL);
        int arp_cheat_recv(uint32_t start, uint32_t endd, tar_info *p=NULL);//net sequence
        int arp_cheat_recv(uint32_t ip, tar_info *p=NULL);//net sequence

        uint32_t arp_cheat(char *start=NULL, char *endsss=NULL);
        uint32_t arp_cheat(uint32_t start, uint32_t endd);//net sequence
        uint32_t arp_cheat(uint32_t ip);//net sequence
        uint32_t arp_cheat(std::vector<tar_info> &vec);
        //uint32_t arp_cheat(char * ip);//net sequence
        int get_mac(char *mac, uint32_t ip);

        int scan_ip_arp( std::vector<tar_info> &vec, uint32_t st, uint32_t ed);//net seq
        int scan_ip_arp(std::vector<tar_info> &vec, char * st=NULL, char * ed=NULL);//net seq


        void show_arp(my_arp *p);
        virtual ~mac_arp();

    protected:

    private:
};


#endif // MAC_ARP_H
