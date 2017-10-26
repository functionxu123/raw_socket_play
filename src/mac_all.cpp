#include "mac_all.h"

/*
第三个参数：protocol协议类型一共有四个，意义如下
ETH_P_IP 0x800 只接收发往本机mac的ip类型的数据帧
ETH_P_ARP 0x806 只接受发往本机mac的arp类型的数据帧
ETH_P_RARP 0x8035 只接受发往本机mac的rarp类型的数据帧
ETH_P_ALL 0x3 接收发往本机mac的所有类型ip arp rarp的数据帧,
                  接收从本机发出的所有类型的数据帧.(混杂模式打开的情况下,会接收到非发往本地mac的数据帧),
*/

mac_all::mac_all(): sock_mac ( htons ( ETH_P_ALL ) ) {
    //ctor
}

mac_all::mac_all(int ind, int send_card): sock_mac ( htons ( ETH_P_ALL ), ind, send_card) {
    //ctor
}

int mac_all::recv_all(char *buff, int len, int flag) {
    socklen_t addrlen = sizeof(struct sockaddr_in);

    int  ret_len = recvfrom ( get_socket(), buff, len, flag, (struct sockaddr*)&caddr, &addrlen ); //
    // int  ret_len = recvfrom ( get_socket(), buff, len, flag, NULL, NULL ); //
    printf("recv form index:%d\n", caddr.sll_ifindex );
    if ( ret_len < 0 ) {
        perror ( "recv_all:recv error!" );
        return -1;
    }
    return ret_len;
}

int mac_all::send_all(char *buf, int len, int flag) {
    int  ret_len = sendto ( get_socket(), buf, len, flag, ( struct sockaddr * ) &saddr, sizeof ( struct sockaddr_ll ) ); //
    if ( ret_len < 0 ) {
        perror ( "send_all::send error!" );
        return -1;
    }
    return ret_len;
}

mac_all::~mac_all() {
    //dtor
}
