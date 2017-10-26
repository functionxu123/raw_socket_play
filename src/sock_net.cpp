#include "sock_net.h"

sock_net::sock_net(int proto):sock_trans(AF_INET,SOCK_RAW,proto)
{
    //ctor
}

sock_net::sock_net(int AF, int type, int proto):sock_trans(AF, type,proto){

}

int sock_net::bind_card(int index){

}

sock_net::~sock_net()
{
    //dtor
}
