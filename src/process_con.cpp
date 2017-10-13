#include "process_con.h"

process_con::process_con()
{
    //ctor
}

int process_con::operator()(int socket, struct sockaddr_in ad){
    printf("comes : %s\n",inet_ntoa(ad.sin_addr));

    return 0;
}

process_con::~process_con()
{
    //dtor
}
