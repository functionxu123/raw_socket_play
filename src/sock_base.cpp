#include "sock_base.h"

int sock_base::local_conf_valid = 0; //这是因为类的静态成员变量在使用前必须先初始化。
local_conf sock_base::local[max_card_num];

sock_base::sock_base(int AF, int type, int proto) {
    socket_m = -1;

    //ctor
    socket_m = socket(AF, type, proto);
    if (socket_m == -1) {
        perror("sock_base:create socket error!");
        exit(1);
    }

    if(!local_conf_valid) {
        local_conf_valid = get_local_info(local);
        if (local_conf_valid <= 0) {
            perror("There is no network card available!");
            exit(1);
        }
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
                //SIOCGIFNETMASK mask
                if(ioctl(sock, SIOCGIFNETMASK, &ifr) >= 0) {
                    char *mas_tep = inet_ntoa(((struct sockaddr_in*) & (ifr.ifr_netmask))->sin_addr);
                    strcpy(p[count].mask, mas_tep);
                } else {
                    perror("    ioctl SIOCGIFNETMASK error");
                    return -1;
                }

                p[count].gate=getgateway();

                count++;
            }
        } else {
            perror("get info error\n");
            return -1;
        }
    }
    return count;
}


int sock_base::get_socket() {
    return socket_m;
}
void sock_base::setsocket(int p) {
    if (socket_m > 0) close(socket_m);
    socket_m = p;
}

uint16_t sock_base::checksum(uint16_t* buffer, int size) { //this size is char size
    unsigned long cksum = 0;
//每16位相加
    while(size > 1)    {
        cksum += *buffer++;
        size -= sizeof(uint16_t);
    }
//最后的奇数字节
    if(size)    {
        cksum += *(uint8_t*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff); //将高16bit与低16bit相加
    cksum += (cksum >> 16);                           //将进位到高位的16bit与低16bit 再相加,确保高16位为0
    return (uint16_t)(~cksum); //最后将结果取反,得到checksum
}
/*

typedef struct my_ip{
    uint8_t ver_hdlen;//version and head length
    uint8_t ser_type;//service type
    uint16_t full_len;//full length of this packet
    uint16_t group_id;//
    uint16_t tag_seg;//tag(3) and differ of segment(13)
    uint8_t timetolive;//
    uint8_t super_proto;//
    uint16_t check_sum;
    uint32_t src_ip;
    uint32_t des_ip;
}my_ip;
*/
void sock_base::form_ip(my_ip *ip, int datalen, int proto,  char *desip, char *srcip, int head_len, int version) {
    bzero(ip, sizeof(my_ip));
    ip->ver_hdlen = (version << 4 | sizeof(my_ip) / sizeof(unsigned long));
    ip->full_len = htons(datalen + head_len);
    ip->group_id = 1; //
    ip->timetolive = 64;
    ip->super_proto = proto & 0xff;
    ip->des_ip = inet_addr(desip);
    if (srcip == NULL) {
        ip->src_ip = inet_addr(local[local_conf_valid - 1].ip);
    } else {
        ip->src_ip = inet_addr(srcip);
    }
    ip->check_sum = checksum((uint16_t*)ip, sizeof(my_ip));
}

void sock_base::form_tcp(my_tcp *tcp, char *data, int data_len, char *src_ip, char *des_ip, int src_port, int des_port, int seq, int ack, char flag,  int hd_len, int win_size) {
    bzero(tcp, sizeof(my_tcp));

    tcp->des_port = htons(des_port);
    tcp->src_port = htons(src_port);

    tcp->hdlen_flag = (hd_len / 4 << 12 | flag);
    tcp->winsize = htons(win_size);
    tcp->ack = htonl(ack);
    tcp->tcp_sequ = htonl(seq);

    char bu[max_ether_len];
    bzero(bu, sizeof(bu));

    fake_hd * fh = (fake_hd*)bu;
    fh->proto = IPPROTO_TCP;
    fh->tcp_len = htons(hd_len + data_len);
    fh->src_ip = inet_addr(src_ip);
    fh->des_ip = inet_addr(des_ip);


    memcpy(bu + sizeof(fake_hd), tcp, sizeof(my_tcp));
    memcpy(bu + sizeof(fake_hd) + sizeof(my_tcp), data, data_len);

    tcp->check_sum = checksum((uint16_t*)bu, sizeof(fake_hd) + sizeof(my_tcp) + data_len);
}

uint32_t sock_base::local_ipstart() {//net seq
    uint32_t mask = htonl(inet_addr(local[local_conf_valid - 1].mask));
    uint32_t ip = htonl(inet_addr(local[local_conf_valid - 1].ip));
    ip &= mask;
    return htonl(ip);
}

uint32_t sock_base::local_ipend() {//net seq
    uint32_t mask = htonl(inet_addr(local[local_conf_valid - 1].mask));
    uint32_t ip = htonl(inet_addr(local[local_conf_valid - 1].ip));
    ip |= (~mask);
    return htonl(ip);
}

char *sock_base::rid_ip(char *p, my_ip*ip) {
    memcpy(ip, p, sizeof(my_ip));

    return p + ((ip->ver_hdlen) & 0x0f) * 4;
}

char *sock_base::rid_tcp(char *p, my_tcp *tcp) {
    memcpy(tcp,p, sizeof(my_tcp));
    return p + (((tcp->hdlen_flag) & 0xf0)>>2);
}

uint32_t sock_base::getgateway(){
    FILE *fp;
    char buf[512];
    char cmd[128];
    char gateway[30];
    char *tmp;

    strcpy(cmd, "ip route");//run ip route command
    fp = popen(cmd, "r");
    if(NULL == fp)
    {
        perror("popen error");
        return -1;
    }
    while(fgets(buf, sizeof(buf), fp) != NULL)
    {
        tmp =buf;
        while(*tmp && (*tmp==' ') )
            ++ tmp;
        if(strncmp(tmp, "default", strlen("default")) == 0)
            break;
    }
    sscanf(buf, "%*s%*s%s", gateway);
    //printf("default gateway:%s\n", gateway);
    pclose(fp);

    return inet_addr(gateway);
}

void sock_base::my_swap_buffer(char *p1, char *p2, int len){
    char tep;
    int i=0;
    for (i=0;i<len;i++){
        tep=p1[i];
        p1[i]=p2[i];
        p2[i]=tep;
    }
}

sock_base::~sock_base() {
    //dtor
    if (socket_m > 0) {
        close(socket_m);
    }
}


