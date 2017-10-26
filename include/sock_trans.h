#ifndef SOCK_TRANS_H
#define SOCK_TRANS_H

#include <sock_base.h>


class sock_trans : public sock_base
{
    public:
        sock_trans(int type=SOCK_STREAM);
        sock_trans(int AF, int type, int proto);
        virtual ~sock_trans();

    protected:

    private:
};

#endif // SOCK_TRANS_H
