#include "udp_client.h"


udp_client::udp_client(uint32_t ip, int port): sock_udp() {
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = ip;//inet_aton("127.0.0.1", (struct in_addr *)&server_addr.sin_addr);
    client_addr.sin_port = htons(port);
}


udp_client::udp_client(char *ip, int port): sock_udp() {
    //ctor
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(ip);//inet_aton("127.0.0.1", (struct in_addr *)&server_addr.sin_addr);
    client_addr.sin_port = htons(port);
}


int udp_client::recv_ser(char *p, int len, int flag) {
st:
    socklen_t addrlen=sizeof(struct sockaddr_in);
    int retlen = recvfrom(get_socket(), p, len, flag, (struct sockaddr*)&host_addr,&addrlen) ;
    if(retlen < 0) {
        if(errno == EINTR)
            return 0;
        else
            perror("udp_client:recv error!");
        return -1;
    }
    return retlen;
}


int udp_client::send_ser(char *p, int len, int flag) {
st:
    int retlen = sendto(get_socket(), p, len, flag, (struct sockaddr*)&client_addr,sizeof(struct sockaddr_in));
    if(retlen < 0) {
        if(errno == EINTR)
            goto st;
        else
            perror("udp_client:send error!");
        return -1;
    }
    return retlen;
}


udp_client::~udp_client() {
    //dtor
}
