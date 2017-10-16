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
        memcpy ( mac->src, local[local_conf_valid-1].mac, mac_len );
    } else {
        memcpy ( mac->src, src, mac_len );
    }
}


char * sock_mac::rid_mac(char *p, my_mac *mac) {
    memcpy(mac, p, sizeof(my_mac));
    return p + sizeof(my_mac);
}


void sock_mac::show_mac ( my_mac*eth ) {
    printf ( "Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->des[0], eth->des[1], eth->des[2], eth->des[3], eth->des[4], eth->des[5] );
    printf ( "Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->src[0], eth->src[1], eth->src[2], eth->src[3], eth->src[4], eth->src[5] );
    printf ( "proto: 0x%2x\n", eth->type );
}

sock_mac::~sock_mac() {
    //dtor
}
