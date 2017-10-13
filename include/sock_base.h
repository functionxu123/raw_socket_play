#ifndef SOCK_BASE_H
#define SOCK_BASE_H


#include "sock_funs.h"

class sock_base
{
    public:
        sock_base(int AF=AF_INET, int type=SOCK_DGRAM, int proto=IPPROTO_IP);
        //
        virtual ~sock_base();
        int get_local_info( local_conf p[]);

    protected:

    private:
        int socket_m;
};

#endif // SOCK_BASE_H
