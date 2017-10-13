#include "sock_base.h"
#include <iostream>

using namespace std;

int main()
{
    sock_base tp;
    local_conf p[10];

   int i= tp.get_local_info(p);
    cout<<i<<":"<<p[i-1].card_name<<" : "<<p[i-1].ip;
    return 0;
}
