#include "vita/vita.h"

#define PORT 8080

void server_simple(void);
void server_with_timeout(void);
int32_t user_get_msg(char *buffer, const size_t buffer_size);

int32_t main(void) {
    VT_LOG_INFO("This is a server device.");

    // server_simple();
    server_with_timeout();

    return 0;
}

void server_simple(void) {
    assert(vt_socket_init() == true);
    {
        VT_LOG_INFO("Starting up the server...");
        const vt_socket_t server_id = vt_socket_startup_server(VT_SOCKET_TYPE_TCP, PORT, 3);
        assert(server_id >= 0);

        // loop: only receive messages
        while (true) {
            VT_LOG_INFO("Waiting for client...");
            const vt_socket_t client_id = vt_socket_accept_client(server_id);
            assert(client_id >= 0);

            VT_LOG_INFO("Client connected. Reading data...");
            static char buffer[VT_STR_TMP_BUFFER_SIZE] = {0};
            const int32_t size_received = vt_socket_receive(client_id, buffer, sizeof(buffer));
            assert(size_received > 0);

            VT_LOG_WARN("Data received: [%s] | bytes: %zu", buffer, size_received);

            // int32_t ret = user_get_msg(buffer, sizeof(buffer));
            // if (ret == -1) break;
            // else if (ret == 0) continue;

            // const size_t msg_len = strlen(buffer) + 1;
            // VT_LOG_INFO("Message entered: [%s] | bytes: %zu", buffer, msg_len);

            // VT_LOG_INFO("Sending message...");
            // const int32_t size_sent = vt_socket_send(client_id, buffer, msg_len);
            // assert(size_sent == msg_len);

            // VT_LOG_INFO("Done.");
        }

        VT_LOG_INFO("Closing the socket...");
        assert(vt_socket_close(server_id) == true);
    }
    assert(vt_socket_quit() == true);
}

void server_with_timeout(void) {
    assert(vt_socket_init() == true);
    {
        VT_LOG_INFO("Starting up the server...");
        const vt_socket_t server_id = vt_socket_startup_server(VT_SOCKET_TYPE_TCP, PORT, 3);
        assert(server_id >= 0);

        // loop: only receive messages
        while (true) {
            /**
             * LISTEN
             */
            // VT_LOG_INFO("Waiting for client...");
            // const vt_socket_t client_id = vt_socket_accept_client(server_id);
            // assert(client_id >= 0);

            // VT_LOG_INFO("Client connected. Reading data...");
            // static char buffer[VT_STR_TMP_BUFFER_SIZE] = {0};
            // const int32_t size_received = vt_socket_receive_timed(client_id, buffer, sizeof(buffer), 5000);
            // assert(size_received >= 0);

            // if (size_received > 0) VT_LOG_WARN("Data received: [%s] | bytes: %zu", buffer, size_received);
            // else VT_LOG_WARN("Nothing received. Listening...");

            /**
             * SEND
             */
            VT_LOG_INFO("Waiting for client...");
            const vt_socket_t client_id = vt_socket_accept_client(server_id);
            assert(client_id >= 0);

            VT_LOG_INFO("Client connected. Sending data...");
            char buffer[4096] = {0}; vt_file_read_to_buffer("/Users/krillos/MyFiles/dev/repos/git.kirillsaidov/zme/assets/order_example.json", buffer, sizeof(buffer));
            const int32_t size_sent = vt_socket_send(client_id, buffer, strlen(buffer));
            assert(size_sent >= 0);

            if (size_sent > 0) VT_LOG_WARN("Data sent: [%s] | bytes: %zu", buffer, size_sent);
            else VT_LOG_WARN("Failed to send a message. Listening again...");
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

