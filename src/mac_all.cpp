#include "mac_all.h"
#include <mac_arp.h>

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
    //printf("recv form index:%d\n", caddr.sll_ifindex );
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

int mac_all::nat(int iner, int out) {
    char buf[max_ether_len];
    char mac_buf[mac_len];
    nat_table ble;
    nat_ite temp;
/**/
    mac_arp ar;
    if(ar.get_mac(mac_buf, local[out].gate) != 0) {
        perror("mac_all:nat:get gate mac error!");
        return -1;
    }
    //printf("Get Gate MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", mac_buf[0] & 0xff, mac_buf[1] & 0xff, mac_buf[2] & 0xff, mac_buf[3] & 0xff, mac_buf[4] & 0xff, mac_buf[5] & 0xff);

////////////////////////////////main nat body
    while(1) {
        int tlen = recv_all(buf);

        if (caddr.sll_ifindex == local[iner].index) {//get from iner

            my_mac *mp = (my_mac*)buf;
            if (mp->type == htons(0x800)) { //ip proto
                //printf("recv:%d\n", tlen);
                my_ip*pp = (my_ip*)rid_mac(buf);
                //show_ip(pp);

                if ((pp->des_ip & inet_addr(local[iner].mask))  == local[iner].gate  ) continue; //iner ip des

                ble.ip = pp->src_ip; //record

                pp->src_ip = inet_addr(local[out].ip);
                memcpy(mp->src, local[out].mac, mac_len);
                memcpy(mp->des, mac_buf, mac_len);

                ble.iner_port = nat_get_port(pp);

                if(ble.iner_port) { //validate
                    if ((temp=nat_find(&ble))!=table.end()){//has existed
                        nat_get_port(pp, (*temp).out_port);//change the src port
                    }else{
                        uint16_t opo = get_freeport();
                        nat_get_port(pp, opo);
                        ble.out_port = opo;
                        table.push_back(ble);
                        printf("add %s:%d --> %d\n", inet_ntoa(i2addr_in(ble.ip)), htons(ble.iner_port), htons(ble.out_port));
                    }
                    /////////prepare to  send
                    checksum_ip_tcp(pp);
                    //show_ip(pp);
                    set_send_card(out);
                    send_all(buf, tlen);//
                }
            } else if(mp->type == htons(0x806)) { //arp
            } else if(mp->type == htons(0x86dd)) { //ipv6
            }
        } else if(caddr.sll_ifindex == local[out].index) { //

        }
    }
    return 0;
}

int mac_all::checksum_ip_tcp(my_ip *p){
    p->check_sum=0;
    p->check_sum=checksum((uint16_t *)p, ((p->ver_hdlen) & 0x0f) * 4);
    printf("check pro 0x%02x :%d\n", p->super_proto&0xff,((p->ver_hdlen) & 0x0f) * 4);

    if (p->super_proto==6){//TCP
        my_tcp *tc=(my_tcp*)rid_ip((char *)p);
        tc->check_sum=0;

        fake_hd fhd;
        bzero(&fhd, sizeof(fake_hd));
        fhd.des_ip=p->des_ip;
        fhd.src_ip=p->src_ip;
        fhd.proto=p->super_proto;
        fhd.tcp_len=htons(htons(p->full_len)-((p->ver_hdlen) & 0x0f) * 4);

        char buff[htons(fhd.tcp_len)+sizeof(fhd)];

        memcpy(buff, &fhd, sizeof(fhd));
        memcpy(buff+sizeof(fhd), tc, htons(fhd.tcp_len));
        tc->check_sum=checksum((uint16_t*)buff, htons(fhd.tcp_len)+sizeof(fhd));
        printf("check tcp %d \n\n", htons(fhd.tcp_len)+sizeof(fhd));
    }
    return 0;
}

nat_ite mac_all::nat_find(nat_table *p){//find if iner net has a reflect
    nat_ite st=table.begin();
    for (; st!=table.end(); st++){
        if((*st).ip == p->ip && (*st).iner_port== p->iner_port)
            return st;
    }
    return st;
}

nat_ite mac_all::nat_find(uint16_t p){//if out port point to a iner addr
    nat_ite st=table.begin();
    for (; st!=table.end(); st++){
        if((*st).out_port== p)
            return st;
    }
    return st;
}

int mac_all::nat_get_port(my_ip *p, uint16_t po) {
    my_tcp*tc = (my_tcp*)rid_ip((char *)p);
    switch(p->super_proto) {
    case 1://icmp
        break;
    case 2://igmp
        break;
    case 6://tcp
    case 17://udp
        uint16_t td = tc->src_port;
        if(po) tc->src_port = po;
        return td;
    }
    return 0;
}

mac_all::~mac_all() {
    //dtor
}
