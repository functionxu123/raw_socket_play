#include "sock_funs.h"


int get_IP_MAC( char *ip, int ip_l, char *mac, int mac_l) { //返回的IP地址为字符串型："192.168.1.1",返回Mac为6个char
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[max_ether_len];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);//create a socket to get mac and ip address
    if (sock == -1) {
        perror("socket error\n");
        return -1;
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) {//ioctl是设备驱动程序中对设备的I/O通道进行管理的函数,获取所有接口的清单
        perror("ioctl error\n");
        close(sock);
        return -1;
    }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));
    char szMac[64];
    int count = 0;
    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);

        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (   (! (ifr.ifr_flags & IFF_LOOPBACK))  && (ifr.ifr_flags & IFF_UP)   ) { // don't count loopback,避免记录环路,仅仅记录up状态的

                if (ioctl(sock, SIOCGIFINDEX, &ifr) == -1) {//return index of card
                    perror ("get index error!");
                    close(sock);
                    return -1;
                }

                success = ifr.ifr_ifindex;


                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {//获取mac地址
                    count ++ ;
                    unsigned char * ptr ;
                    ptr = (unsigned char  *)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0];

                    if (mac != NULL && mac_l >= mac_len)
                        memcpy(mac, ptr, mac_len);

                    snprintf(szMac, 64, "%02X:%02X:%02X:%02X:%02X:%02X", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3), *(ptr + 4), *(ptr + 5));
                    //printf("%d,Interface name : %s , Mac address : %s \n", count, ifr.ifr_name, szMac);
                } else {
                    perror("get mac error!");
                    close(sock);
                    return -1;
                }

                if (ioctl(sock, SIOCGIFADDR, &ifr) == -1 ) { //get  ip address
                    perror("ioctl() get ip");
                    close(sock);
                    return -1;
                } else {
                    char *src_ip = inet_ntoa(((struct sockaddr_in *) & (ifr.ifr_addr))->sin_addr);

                    if (ip != NULL  && ip_l > strlen(src_ip))
                        strcpy(ip, src_ip);

                    //printf("local ip:%s\n", src_ip);
                }

            }
        } else {
            perror("get info error\n");
            close(sock);
            return -1;
        }
    }
    close(sock);
    return success;
}



void getandparse_arp() {
    char buffer[max_ether_len];
    int sock, n;
    struct ethhdr *eth;
    my_arp *arp;

    if (0 > (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP)))) {
        perror("socket");
        close(sock);
        exit(1);
    }
rec:
    memset(buffer, 0, max_ether_len);
    n = recvfrom(sock, buffer, max_ether_len, 0, NULL, NULL);

    printf("\n\n\nrecv %d Bytes\n", n);
    eth = (struct ethhdr*)buffer;
    printf("Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);

    arp = (my_arp*)(buffer + sizeof(struct ethhdr));
    if(arp->op_type == 1) printf("arp request!\n");
    else if (arp->op_type == 2) printf("arp response!\n");

    printf("src ip:%s\n", inet_ntoa(   *((struct in_addr*) & (arp->src_ip))  ));
    printf("des ip:%s\n", inet_ntoa(  *((struct in_addr*) & (arp->des_ip))  ));

    printf("src mac in packet:%02x:%02x:%02x:%02x:%02x:%02x\n", arp->src_mac[0], arp->src_mac[1], arp->src_mac[2], arp->src_mac[3], arp->src_mac[4], arp->src_mac[5]);
    printf("des mac in packet:%02x:%02x:%02x:%02x:%02x:%02x\n", arp->des_mac[0], arp->des_mac[1], arp->des_mac[2], arp->des_mac[3], arp->des_mac[4], arp->des_mac[5]);

    //goto rec;
    close(sock);
}


int fill_mac_arp(my_mac *p) {
    char mac_cro[mac_len] = BROADCAST_ADDR;
    memcpy(p->des, mac_cro, mac_len);

    get_IP_MAC(NULL, 0, p->src, mac_len);
    p->type = htons(ETHERTYPE_ARP);

    return 0;
}

int fill_arp(my_arp*p) {
    p->hd_type = htons(ARPHRD_ETHER);
    p->pro_type = htons(ETHERTYPE_IP);
    p->mac_length = mac_len;
    p->ip_length = ip_len;
    p->op_type = htons(ARPOP_REPLY);//ARPOP_REQUEST

    get_IP_MAC(NULL, 0, p->src_mac, mac_len);

    char mac_cro[60] = {0};
    //memcpy(p->des_mac,mac_cro,mac_len);
    memset(p->des_mac, 0, mac_len);//destion mac is all zero

    get_IP_MAC(mac_cro, 60, NULL, 0);
   // printf("fil_arp_ip:%s\n", mac_cro);

   #if 0
    p->src_ip = inet_addr(mac_cro);
    #else
    p->src_ip = inet_addr("10.137.0.1");
    #endif // 1
    p->des_ip = inet_addr("10.137.225.51");

    return 0;
}


int send_arp() {
    struct sockaddr_ll saddr;
    int sock_raw_fd;
    int ret_len = 0;

    my_mac ma;
    my_arp ar;

    char buff[max_ether_len] = {0};

    memset(&saddr, 0, sizeof(struct sockaddr_ll));

    saddr.sll_ifindex = get_IP_MAC(NULL, 0, NULL, 0);
    //printf("ifindex:%d\n", saddr.sll_ifindex);

    saddr.sll_family = PF_PACKET;

    fill_mac_arp(&ma);
    fill_arp(&ar);

    memcpy(buff, &ma, sizeof(my_mac));
    memcpy(buff + sizeof(my_mac), &ar, sizeof(my_arp));


    if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1) {
        perror("create send socket error!");
        return -1;
    }

    for (int i = 0; i < sizeof(my_mac) + sizeof(my_arp); i++)
        printf("%02x ", buff[i] & 0xff);


    ret_len = sendto(sock_raw_fd, buff, sizeof(my_mac) + sizeof(my_arp), 0, (struct sockaddr *)&saddr, sizeof(struct sockaddr_ll));//
    if (ret_len < 0) {
        perror("send error!");
        close(sock_raw_fd);
        return -1;
    }
    printf("send data:%d Bytes\n", ret_len);
    close(sock_raw_fd);

    return 0;
}
/*
int main() {
    char p[100];
    get_IP_MAC(p, 100, NULL, 0);
    printf("%s\n", p);

    while(1){
        send_arp();
        sleep(1);
    }

    getandparse_arp();

    return 0;
}
*/
