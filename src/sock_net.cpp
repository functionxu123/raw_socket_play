#include "sock_net.h"

sock_net::sock_net(int proto):sock_base(AF_INET,SOCK_RAW,proto)
{
    //ctor
}

sock_net::~sock_net()
{
    //dtor
}
