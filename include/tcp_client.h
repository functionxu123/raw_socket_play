#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <sock_tcp.h>
#include <sock_funs.h>

/*
   ser_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", (struct in_addr *)&ser_addr.sin_addr);
    ser_addr.sin_port = htons(SERVER_PORT);

    connect(sock_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr))
*/

class tcp_client : public sock_tcp {
public:
    tcp_client(char *ip, int p);
    tcp_client(uint32_t ip, int p);
    int connect_ser();
    int recv_ser(char *p,int len, int flag=0);
    int send_ser(char *p, int len, int f=0);
    virtual ~tcp_client();

protected:

private:
    //truct sockaddr_in server_addr;
};

#endif // TCP_CLIENT_H
