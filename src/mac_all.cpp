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

mac_all::mac_all(int ind): sock_mac ( htons ( ETH_P_ALL ) , ind) {
    //ctor
}

int mac_all::recv_all(char *buff, int flag) {
    int  ret_len = recvfrom ( get_socket(), buff, max_ether_len, flag, NULL, NULL ); //
    if ( ret_len < 0 ) {
        perror ( "send error!" );
        return -1;
    }
    return ret_len;
}

mac_all::~mac_all() {
    //dtor
}
