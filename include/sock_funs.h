#ifndef SOCK_FUNS_H_MY
#define SOCK_FUNS_H_MY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netpacket/packet.h>

#define max_ether_len ( 2048)
#define mac_len   ( 6)
#define ip_len (4)


#define BROADCAST_ADDR  {0xff,0xff,0xff,0xff,0xff,0xff}

#pragma pack (1)//编译器将按照n个字节对齐
typedef struct my_mac{
    uint8_t des[mac_len];//destination mac address?all 1?
    uint8_t src[mac_len];//source mac address
    uint16_t type;//0x0806->arp; ox0800->ip
}my_mac;

typedef struct my_arp{
    uint16_t hd_type;//harware type:1->ethernet
    uint16_t pro_type;//proto type:0x0800->ip
    uint8_t mac_length;//mac length  6
    uint8_t ip_length;//ip length  4
    uint16_t op_type;//operation type:1->arp request;2->arp response


    uint8_t src_mac[mac_len];//source mac address
    uint32_t src_ip;//source ip address
    uint8_t des_mac[mac_len];//destination mac address:all zero in arp request
    uint32_t des_ip;//destination ip address

}my_arp;


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

typedef struct my_tcp{
    uint16_t src_port;
    uint16_t des_port;
    uint32_t tcp_sequ;//tcp sequence
    uint32_t ack;//ack num
    uint16_t hdlen_flag;//head length(4)+remain(6)+flag(6)
    uint16_t winsize;//window size
    uint16_t check_sum;//tcp check sum
    uint16_t urge;//if urge
}my_tcp;

#pragma pack ()//取消自定义字节对齐方式

typedef struct {//pc's para
    char ip[32];
    char mac[mac_len];
    int index;
    char card_name[IFNAMSIZ];
}local_conf;

int get_IP_MAC( char *ip,int ip_l,char *mac,int mac_l) ;//返回的IP地址为字符串型："192.168.1.1",返回Mac为6个char

int fill_mac_arp(my_mac *p);

int fill_arp(my_arp*p);

void getandparse_arp();



#endif
