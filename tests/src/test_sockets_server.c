#include "vita/vita.h"

int32_t main(void) {
    printf("server: hello!\n");

    assert(vt_socket_init() == true);
    {
        VT_LOG_INFO("Starting up the server...");
        vt_socket_t server_id = vt_socket_startup_server();
    }
    assert(vt_socket_quit() == true);
    return 0;
}

