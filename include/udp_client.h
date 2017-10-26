#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <sock_udp.h>


class udp_client : public sock_udp {
public:
    udp_client(char *ip, int port);
    udp_client(uint32_t ip, int port);
    int recv_ser(char *p, int len, int flag = 0);
    int send_ser(char *p, int len, int f = 0);
    virtual ~udp_client();

protected:

private:
    //struct sockaddr_in server_addr;
};

#endif // UDP_CLIENT_H
