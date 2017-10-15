#ifndef SOCK_BASE_H
#define SOCK_BASE_H


#include "sock_funs.h"


class sock_base {
public:
    sock_base(int AF = AF_INET, int type = SOCK_DGRAM, int proto = IPPROTO_IP);
    //
    virtual ~sock_base();
    int get_local_info( local_conf p[]);

    int get_socket();
    void setsocket(int p);

    static int local_conf_valid;//这是因为类的静态成员变量在使用前必须先初始化。
    static local_conf local[max_card_num];

protected:


private:
   int socket_m;

};

#endif // SOCK_BASE_H
