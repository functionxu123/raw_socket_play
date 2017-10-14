#ifndef SOCK_NET_H
#define SOCK_NET_H

#include <sock_base.h>

/*
 int socketfd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);//在网络层使用的原始套接字
   int socketfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP));//在链路层使用
*/

class sock_net : public sock_base
{
    public:
        sock_net();
        virtual ~sock_net();

    protected:

    private:
};

#endif // SOCK_NET_H
