#include "sock_mac.h"

sock_mac::sock_mac(int proto): sock_base(PF_PACKET, SOCK_RAW, proto) {
    //ctor
}

void sock_mac::form_machd(my_mac* mac,  uint16_t type, char *des, char *src) {
    mac->type = type;
    if (des == NULL) {
        char mac_cro[mac_len] = BROADCAST_ADDR;
        memcpy(mac->des, mac_cro, mac_len);
    }else{
        memcpy(mac->des, des, mac_len);
    }

    if (src==NULL){
        local_conf tp;
        get_local_info(&tp);
        memcpy(mac->src, tp.mac, mac_len);
    }else{
        memcpy(mac->src, src, mac_len);
    }
}

sock_mac::~sock_mac() {
    //dtor
}
