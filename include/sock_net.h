#ifndef SOCK_NET_H
#define SOCK_NET_H

#include <sock_trans.h>

/*
 int socketfd = socket(AF_INET,SOCK_RAW,    IPPROTO_TCP/IPPROTO_UDP/IPPROTO_ICMP);//在网络层使用的原始套接字
   int socketfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP));//在链路层使用

*/

class sock_net : public sock_trans
{
    public:
        sock_net(int proto);
        sock_net(int AF, int type, int proto);
        virtual int bind_card(int index=local_conf_valid-1);
        virtual ~sock_net();

    protected:

    private:
};

#endif // SOCK_NET_H
