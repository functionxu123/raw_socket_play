#ifndef PROCESS_CON_H
#define PROCESS_CON_H
#include "sock_funs.h"
#include <iostream>

class process_con
{
    public:
        process_con();
        virtual int operator()(int socket, struct sockaddr_in ad);
        virtual ~process_con();

    protected:

    private:
};

#endif // PROCESS_CON_H
