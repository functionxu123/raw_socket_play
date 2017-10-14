#ifndef SOCK_MAC_H
#define SOCK_MAC_H

#include <sock_base.h>


class sock_mac : public sock_base
{
    public:
        sock_mac(int proto);
        void form_machd(my_mac *mac, uint16_t type=htons(ETHERTYPE_ARP), char *des=NULL, char *src=NULL );
        virtual ~sock_mac();

    protected:

    private:
};

#endif // SOCK_MAC_H
