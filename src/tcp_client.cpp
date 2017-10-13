#include "tcp_client.h"
/*
   ser_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", (struct in_addr *)&ser_addr.sin_addr);
    ser_addr.sin_port = htons(SERVER_PORT);

    connect(sock_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr))
*/

tcp_client::tcp_client(char *p, int port): sock_tcp() {
    //ctor
    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(p);//inet_aton("127.0.0.1", (struct in_addr *)&server_addr.sin_addr);
    server_addr.sin_port = htons(port);
}

tcp_client::tcp_client(uint32_t p, int port): sock_tcp() {
    //ctor
    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = p;
    server_addr.sin_port = htons(port);
}

int tcp_client::connect_ser() {
    int tp = connect(get_socket(), (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    if(tp != 0) {
        perror("tcp_client:connect error!");
        return -1;
    }
    return tp;
}

int tcp_client::recv_ser(char *p, int len, int flag) {
st:
    int retlen = recv(get_socket(), p, len, flag) ;
    if(retlen < 0) {
        if(errno == EINTR)
            return 0;
        else
            perror("tcp_client:recv error!");
        return -1;
    }
    return retlen;
}


int tcp_client::send_ser(char *p, int len, int flag) {
st:
    int retlen = send(get_socket(), p, len, flag);
    if(retlen < 0) {
        if(errno == EINTR)
            goto st;
        else
            perror("tcp_client:send error!");
        return -1;
    }
    return retlen;
}

tcp_client::~tcp_client() {
    //dtor
}
