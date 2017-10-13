#include <tcp_client.h>
#include <tcp_server.h>
#include <sock_funs.h>

int main(){
    /*
    tcp_server s(6789);
    s.bind_addr();
    s.start_listen();
    s.accept_block();
    */
    char *p="127.0.0.1";
    tcp_client t(p,6789);
    t.connect_ser();
    while (1) {
            t.send_ser(p,10);
            sleep(1);
    }
    return 0;
}
