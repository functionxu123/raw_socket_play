#include "sock_trans.h"

sock_trans::sock_trans(int type):sock_base(AF_INET,type,0)
{
    //ctor
}

sock_trans::~sock_trans()
{
    //dtor
}
