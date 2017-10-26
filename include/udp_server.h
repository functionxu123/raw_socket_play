#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <sock_udp.h>
#include <process_con.h>


class udp_server : public sock_udp
{
    public:
        udp_server(int p);
        udp_server(int , int);
        int bind_addr();
        int bind_addr(sockaddr_in *p);

        int accept_block(process_con pro=process_con(),int flag=0);

        virtual ~udp_server();

    protected:

    private:
        //struct sockaddr_in host_addr;
};

#endif // UDP_SERVER_H
