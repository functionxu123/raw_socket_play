#include "sock_base.h"

sock_base::sock_base(int AF, int type, int proto) {
    socket_m = -1;

    //ctor
    socket_m = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (socket_m == -1) {
        perror("sock_base:create socket error!");
        exit(1);
    }
}

int sock_base::get_local_info( local_conf p[]) {
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[max_ether_len];
    const int sock = socket_m;

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) {//ioctl是设备驱动程序中对设备的I/O通道进行管理的函数,获取所有接口的清单
        perror("ioctl error\n");
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

                strcpy( (p[count]).card_name, it->ifr_name);

                if (ioctl(sock, SIOCGIFINDEX, &ifr) == -1) {//return index of card
                    perror ("get index error!");
                    return -1;
                }

                p[count].index = ifr.ifr_ifindex;


                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {//获取mac地址
                    unsigned char * ptr ;
                    ptr = (unsigned char  *)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0];

                    memcpy((p[count].mac), ptr, mac_len);

                    //snprintf(szMac, 64, "%02X:%02X:%02X:%02X:%02X:%02X", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3), *(ptr + 4), *(ptr + 5));
                    //printf("%d,Interface name : %s , Mac address : %s \n", count, ifr.ifr_name, szMac);
                } else {
                    perror("get mac error!");
                    return -1;
                }

                if (ioctl(sock, SIOCGIFADDR, &ifr) == -1 ) { //get  ip address
                    perror("ioctl() get ip");
                    return -1;
                } else {
                    char *src_ip = inet_ntoa(((struct sockaddr_in *) & (ifr.ifr_addr))->sin_addr);
                    strcpy(  (p[count]).ip,   src_ip);
                    //printf("local ip:%s\n", src_ip);
                }
                count++;
            }
        } else {
            perror("get info error\n");
            return -1;
        }
    }
    return count;
}


sock_base::~sock_base() {
    //dtor
    if (socket_m > 0) {
        close(socket_m);
    }
}


