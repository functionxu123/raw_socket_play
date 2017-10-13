#include "process_con.h"

process_con::process_con() {
    //ctor
}

int process_con::operator()(int socket, struct sockaddr_in ad) {
    printf("comes : %s\n", inet_ntoa(ad.sin_addr));

    int retlen;
    char p[200];
    while (1) {
st:
        retlen = recv(socket, p, 200, 0) ;
        if(retlen < 0) {
            return -1;
        }
        //return retlen;
        printf("%s\n",p);
    }
    return retlen;
}

int process_con::operator()(int socket, struct sockaddr_in ad,char *p,int len){

    printf("get %s from %s\n",p,inet_ntoa(ad.sin_addr));
    return 0;
}

process_con::~process_con() {
    //dtor
}
