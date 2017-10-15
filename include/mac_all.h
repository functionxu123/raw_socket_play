#ifndef MAC_ALL_H
#define MAC_ALL_H

#include <sock_mac.h>


class mac_all : public sock_mac {
public:
    mac_all();
    int recv_all(char *buf, int flag=0);


    virtual ~mac_all();

protected:

private:

};

#endif // MAC_ALL_H
