#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <sock_tcp.h>
#include "process_con.h"
/*
  hostaddr.sin_family = AF_INET;
  hostaddr.sin_port = htons(LISTEN_PORT);
  hostaddr.sin_addr.s_addr = htonl(INADDR_ANY);//server
*/

class tcp_server : public sock_tcp
{
    public:
        tcp_server(int port);
        virtual ~tcp_server();
        int bind_addr();
        int bind_addr(sockaddr_in *p);

        int start_listen(int num=MAX_LISTEN_NUM);

        int accept_block(process_con pro=process_con());

    protected:

    private:
        struct sockaddr_in host_addr;
};

#endif // TCP_SERVER_H
