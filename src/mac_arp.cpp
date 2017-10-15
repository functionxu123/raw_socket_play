#include "mac_arp.h"


/*
第三个参数：protocol协议类型一共有四个，意义如下
ETH_P_IP 0x800 只接收发往本机mac的ip类型的数据帧
ETH_P_ARP 0x806 只接受发往本机mac的arp类型的数据帧
ETH_P_ARP 0x8035 只接受发往本机mac的rarp类型的数据帧
ETH_P_ALL 0x3 接收发往本机mac的所有类型ip arp rarp的数据帧,
                  接收从本机发出的所有类型的数据帧.(混杂模式打开的情况下,会接收到非发往本地mac的数据帧),
*/

mac_arp::mac_arp() : sock_mac ( htons ( ETH_P_ARP ) ) { //注意，mac的socket第三个参数要为htons()过得
    //ctor
}

//void form_arp(char * ip_src,  char *ip_des, int op=1, char *mac_src=NULL, char * mac_des=NULL);
void mac_arp::form_arp ( my_arp *p, char * ip_src,  char *ip_des, int op, char *mac_src, char * mac_des ) {
    p->hd_type = htons ( ARPHRD_ETHER );
    p->pro_type = htons ( ETHERTYPE_IP );
    p->mac_length = mac_len;
    p->ip_length = ip_len;
    p->op_type = htons ( op ); //ARPOP_REQUEST    ARPOP_REPLY

    //get_IP_MAC(NULL, 0, p->src_mac, mac_len);
    if ( mac_src == NULL ) { //mac src
        memcpy ( p->src_mac, local[local_conf_valid - 1].mac, mac_len );
    } else {
        memcpy ( p->src_mac, mac_src, mac_len );
    }

    if ( mac_des == NULL ) { //set mac destionation
        memset ( p->des_mac, 0, mac_len ); //destion mac is all zero
    } else {
        memcpy ( p->des_mac, mac_des, mac_len );
    }
    p->src_ip = inet_addr ( ip_src );
    p->des_ip = inet_addr ( ip_des );
}

int mac_arp::send_arp ( my_mac * mac, my_arp*arp, int flag ) {
    char buff[sizeof ( my_mac ) + sizeof ( my_arp )];
    memcpy ( buff, mac, sizeof ( my_mac ) );
    memcpy ( buff + sizeof ( my_mac ), arp, sizeof ( my_arp ) );

    int  ret_len = sendto ( get_socket(), buff, sizeof ( my_mac ) + sizeof ( my_arp ), flag, ( struct sockaddr * ) &saddr, sizeof ( struct sockaddr_ll ) ); //
    if ( ret_len < 0 ) {
        perror ( "send error!" );
        return -1;
    }
    printf ( "send data:%d Bytes\n", ret_len );
    return ret_len;
}

int mac_arp::recv_arp ( my_mac * mac, my_arp*arp, int flag ) {
    char buff[max_ether_len];

    int  ret_len = recvfrom ( get_socket(), buff, max_ether_len, flag, NULL, NULL ); //
    if ( ret_len < 0 ) {
        perror ( "send error!" );
        return -1;
    }
    memcpy ( mac, buff, sizeof ( my_mac ) );
    memcpy ( arp, buff + sizeof ( my_mac ), sizeof ( my_arp ) );
    return ret_len;
}

void mac_arp::show_mac ( my_mac*eth ) {
    printf ( "Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->des[0], eth->des[1], eth->des[2], eth->des[3], eth->des[4], eth->des[5] );
    printf ( "Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->src[0], eth->src[1], eth->src[2], eth->src[3], eth->src[4], eth->src[5] );
    printf ( "proto: 0x%2x", eth->type );
}
void mac_arp::show_arp ( my_arp *arp ) {
    if(arp->op_type == 1) printf("arp request(1)\n");
    else if (arp->op_type == 2) printf("arp response(2)\n");

    printf ( "src ip:%s\n", inet_ntoa (   * ( ( struct in_addr* ) & ( arp->src_ip ) )  ) );
    printf ( "des ip:%s\n", inet_ntoa (  * ( ( struct in_addr* ) & ( arp->des_ip ) )  ) );
    printf ( "src mac in packet:%02x:%02x:%02x:%02x:%02x:%02x\n", arp->src_mac[0], arp->src_mac[1], arp->src_mac[2], arp->src_mac[3], arp->src_mac[4], arp->src_mac[5] );
    printf ( "des mac in packet:%02x:%02x:%02x:%02x:%02x:%02x\n", arp->des_mac[0], arp->des_mac[1], arp->des_mac[2], arp->des_mac[3], arp->des_mac[4], arp->des_mac[5] );
}

mac_arp::~mac_arp() {
    //dtor
}
