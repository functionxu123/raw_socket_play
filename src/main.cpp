#include <tcp_client.h>
#include <tcp_server.h>
#include <sock_funs.h>
#include <sock_base.h>
#include <mac_arp.h>

int main(){
    mac_arp mya;
    my_mac m;
    my_arp a;
    //mya.recv_arp(&m, &a);

    //mya.show_arp(&a);

    mya.form_arp(&a,"10.137.0.1","10.137.225.51",2);
    mya.form_machd(&m);

    while(1){
    mya.send_arp(&m, &a);
    sleep(1);
    }
    return 0;
}
