#include "udp_server.h"

udp_server::udp_server(int port): sock_udp() {
    //ctor
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);//servers
}

udp_server::udp_server(int port, int index): sock_udp() {
    //ctor
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = inet_addr(local[index].ip);//servers
}

int udp_server::bind_addr() {
    if(bind( get_socket(), (struct sockaddr *)&host_addr, sizeof(struct sockaddr_in)) < 0) {
        perror("udp_server:bind port error!");
        return -1;
    }
    return 0;
}
int udp_server::bind_addr(sockaddr_in *p) {
    if(bind(get_socket(), (struct sockaddr *)&p, sizeof(struct sockaddr_in)) < 0) {
        perror("udp_server:bind port error!");
        return -1;
    }
    return 0;
}

int udp_server::accept_block(process_con pro,int flag)
{
    socklen_t addrlen=sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;

    char p[128];

    int num = recvfrom(get_socket(), p, 128, flag, (struct sockaddr*)&client_addr, &addrlen);

    if (num<0){
        perror("udp_server:recv_from error!");
        return -1;
    }

    return pro(get_socket(), client_addr, p, 128) ;
}

udp_server::~udp_server() {
    //dtor
}
