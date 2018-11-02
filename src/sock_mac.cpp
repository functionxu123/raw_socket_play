#include "sock_mac.h"

sock_mac::sock_mac ( int proto ) : sock_net( PF_PACKET, SOCK_RAW, proto ) {
    //ctor
    memset ( &saddr, 0, sizeof ( struct sockaddr_ll ) );
    saddr.sll_ifindex = local[local_conf_valid - 1].index;//0 means all the port
    saddr.sll_family = PF_PACKET;
    saddr.sll_protocol = proto;

    memset ( &caddr, 0, sizeof ( struct sockaddr_ll ) );
    caddr.sll_ifindex = local[local_conf_valid - 1].index;//0 means all the port
    caddr.sll_family = PF_PACKET;
    caddr.sll_protocol = proto;

    sel_send_card=local_conf_valid - 1;
    sel_recv_card=local_conf_valid - 1;
}

sock_mac::sock_mac ( int proto, int ind, int sen) : sock_net ( PF_PACKET, SOCK_RAW, proto ) {
    //ctor
    memset ( &saddr, 0, sizeof ( struct sockaddr_ll ) );
    saddr.sll_ifindex = local[ ind ].index;
    saddr.sll_family = PF_PACKET;
    saddr.sll_protocol = proto;
    sel_send_card=ind;

    memset ( &caddr, 0, sizeof ( struct sockaddr_ll ) );
    caddr.sll_ifindex = local[sen].index; //local[local_conf_valid - 1].index;//0 means all the port
    caddr.sll_family = PF_PACKET;
    caddr.sll_protocol = proto;
    sel_recv_card=sen;
}

/*
CAN ONLY CALLED ONCE, BECAUSE IT'S RELY ON BIND, BIND ONLY ONCE
*/
int sock_mac::set_recv_card(int ind) {
    caddr.sll_ifindex = local[ind].index;
    sel_recv_card=ind;
    if(bind (get_socket(), (struct sockaddr *)&caddr, sizeof(struct sockaddr_ll)) != 0) {
        perror("sock_mac:bind error!");
        return -1;
    }
    return 0;
}

int sock_mac::set_send_card(int ind) {
    saddr.sll_ifindex = local[ind].index;
    sel_send_card=ind;
    return 0;
}

void sock_mac::form_machd ( my_mac *mac, char *src, char *des,  uint16_t type ) {
    mac->type = type;
    if ( des == NULL ) {
        unsigned char mac_cro[mac_len] = BROADCAST_ADDR;
        memcpy ( mac->des, mac_cro, mac_len );
    } else {
        memcpy ( mac->des, des, mac_len );
    }

    if ( src == NULL ) {
        memcpy ( mac->src, local[sel_send_card].mac, mac_len );
    } else {
        memcpy ( mac->src, src, mac_len );
    }
}

char * sock_mac::rid_mac(char *p, my_mac *mac) {
    if (mac != NULL) memcpy(mac, p, sizeof(my_mac));
    return p + sizeof(my_mac);
}

void sock_mac::show_mac ( my_mac*eth ) {
    printf ( "%02x:%02x:%02x:%02x:%02x:%02x --> ", eth->des[0], eth->des[1], eth->des[2], eth->des[3], eth->des[4], eth->des[5] );
    printf ( "%02x:%02x:%02x:%02x:%02x:%02x\n", eth->src[0], eth->src[1], eth->src[2], eth->src[3], eth->src[4], eth->src[5] );
    printf ( "proto: 0x%2x\n", htons(eth->type) );
}

sock_mac::~sock_mac() {
    //dtor
}
