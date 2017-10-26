#ifndef SERVER_OPS_H
#define SERVER_OPS_H
#include <sock_funs.h>

class server_ops
{
    public:
        server_ops(int port);

        //int bind_addr();
        virtual ~server_ops();
        struct sockaddr_in host_addr;
    protected:

    private:

};

#endif // SERVER_OPS_H
