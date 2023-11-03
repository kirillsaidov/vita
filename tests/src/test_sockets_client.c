#include "vita/vita.h"

#define PORT 8080
#define IP_ADDRESS "127.0.0.1"

void client_simple(void);
int32_t user_get_msg(char *buffer, const size_t buffer_size);

int32_t main(void) {
    VT_LOG_INFO("This is a client device.");

    client_simple();

    return 0;
}

void client_simple(void) {
    assert(vt_socket_init() == true);
    {
        VT_LOG_INFO("Starting up the client and connecting to server...");
        const struct VitaSocketAddress server_address = vt_socket_make_address(PORT, IP_ADDRESS);
        const vt_socket_t server_id = vt_socket_startup_client(VT_SOCKET_TYPE_TCP, server_address);
        assert(server_id >= 0);

        // loop: only send messagges
        while (true) {
            VT_LOG_INFO("Connected.");
            static char buffer[VT_STR_TMP_BUFFER_SIZE] = {0};
            int32_t ret = user_get_msg(buffer, sizeof(buffer));
            if (ret == -1) break;
            else if (ret == 1) {
                const size_t msg_len = strlen(buffer) + 1;
                VT_LOG_INFO("Message entered: [%s] | bytes: %zu", buffer, msg_len);

                VT_LOG_INFO("Sending message...");
                const int32_t size_sent = vt_socket_send(server_id, buffer, msg_len);
                assert(size_sent == msg_len);
            }

            // VT_LOG_INFO("Waiting for server reply...");
            // const int32_t size_received = vt_socket_receive(server_id, buffer, sizeof(buffer));
            // assert(size_received > 0);

            // VT_LOG_WARN("Data received: [%s] | bytes: %zu", buffer, size_received);
        }

        VT_LOG_INFO("Closing the socket...");
        assert(vt_socket_close(server_id) == true);
    }
    assert(vt_socket_quit() == true);
}

// returns -1:quit, 0:skip, 1:otherwise
int32_t user_get_msg(char *buffer, const size_t buffer_size) {
    VT_LOG_INFO("Send message? (Yes/Skip/Quit): ");

    // read answer
    char c;
    scanf(" %c", &c);
    c = tolower(c);
    if (c == 'y') {
        VT_LOG_INFO("Type message: ");
        scanf(" %s", buffer);
        return 1;
    } else if (c == 'q') {
        return -1;
    } else {
        return 0;
    }
}

