#include "sock_mac.h"

sock_mac::sock_mac ( int proto ) : sock_base ( PF_PACKET, SOCK_RAW, proto ) {
    //ctor
    memset ( &saddr, 0, sizeof ( struct sockaddr_ll ) );
    saddr.sll_ifindex = local[local_conf_valid - 1].index;
    saddr.sll_family = PF_PACKET;
}

void sock_mac::form_machd ( my_mac *mac, char *src, char *des,  uint16_t type ) {
    mac->type = type;
    if ( des == NULL ) {
        char mac_cro[mac_len] = BROADCAST_ADDR;
        memcpy ( mac->des, mac_cro, mac_len );
    } else {
        memcpy ( mac->des, des, mac_len );
    }

    if ( src == NULL ) {
        local_conf tp;
        get_local_info ( &tp );
        memcpy ( mac->src, tp.mac, mac_len );
    } else {
        memcpy ( mac->src, src, mac_len );
    }
}


char * sock_mac::rid_mac(char *p, my_mac *mac) {
    memcpy(mac, p, sizeof(my_mac));
    return p + sizeof(my_mac);
}

char *sock_mac::rid_ip(char *p, my_ip*ip) {
    memcpy(ip, p, sizeof(my_ip));

    return p + ((ip->ver_hdlen) & 0x0f) * 4;
}
char *sock_mac::rid_tcp(char *p, my_tcp *tcp) {
    memcpy(tcp,p, sizeof(my_tcp));
    return p + (((tcp->hdlen_flag) & 0xf0)>>2);
}

sock_mac::~sock_mac() {
    //dtor
}
