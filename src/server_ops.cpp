#include "server_ops.h"

server_ops::server_ops(int port)
{
    //ctor
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);//server
}


server_ops::~server_ops()
{
    //dtor
}
