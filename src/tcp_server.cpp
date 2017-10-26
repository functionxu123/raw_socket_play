#include "tcp_server.h"

tcp_server::tcp_server(int port): sock_tcp() {
    //ctor
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);//servers
}

tcp_server::tcp_server(int port, int index): sock_tcp() {
    //ctor
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = inet_addr(local[index].ip);//servers
}

int tcp_server::bind_addr() {
    if(bind( get_socket(), (struct sockaddr *)&host_addr, sizeof(struct sockaddr_in)) < 0) {
        perror("tcp_server:bind port error!");
        return -1;
    }
    return 0;
}

int tcp_server::bind_addr(sockaddr_in *p) {
    if(bind(get_socket(), (struct sockaddr *)&p, sizeof(struct sockaddr_in)) < 0) {
        perror("tcp_server:bind port error!");
        return -1;
    }
    return 0;
}

int tcp_server::start_listen(int num) {
    if(listen(get_socket(), num) < 0) {
        perror("tcp_server:listen error!");
        printf ("error %d\n",errno);
        return -1;
    }
    return 0;
}

int tcp_server::accept_block(process_con pro) {
    int client_sock;
    struct sockaddr_in client_addr;
    int socklen=sizeof(struct sockaddr);
    client_sock = accept(get_socket(), (struct sockaddr *)&client_addr, (socklen_t*)&socklen);

    if (client_sock<0){
        perror("tcp_server:accept error!");
        return -1;
    }
    return pro(client_sock,client_addr);
}

tcp_server::~tcp_server() {
    //dtor
}
