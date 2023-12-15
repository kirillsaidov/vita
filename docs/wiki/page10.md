# Working with sockets

Vita provides a minimal sockets wrapper, but still allows for additonal configuration with the original API. It uses `winsock2` on windows and POSIX sockets on unix platforms. For more details refer to [`network/sockets.h`](../../inc/vita/network/sockets.h).

## Contents
1. [Server](page10.md#server)
2. [Client](page10.md#client)

### Server

```c
assert(vt_socket_init() == true);
{
    // create server
    const vt_socket_t server_id = vt_socket_startup_server(VT_SOCKET_TYPE_TCP, PORT, /*backlog*/ 3);
    
    // wait for clients
    while (true) {
        const vt_socket_t client_id = vt_socket_accept_client(server_id);

        // receive
        static char buffer[VT_STR_TMP_BUFFER_SIZE] = {0};
        const int32_t size_received = vt_socket_receive(client_id, buffer, sizeof(buffer));

        // receive timed
        // const int32_t size_received = vt_socket_receive_timed(client_id, buffer, sizeof(buffer), 5000);

        // send
        const char *msg = "hello - from server.";
        const size_t msg_len = strlen(msg);
        const int32_t size_sent = vt_socket_send(client_id, msg, msg_len);
    }

    // shutdown
    vt_socket_close(server_id);
}
assert(vt_socket_quit() == true);
```

### Client

```c
assert(vt_socket_init() == true);
{
    // connect to server
    const struct VitaSocketAddress server_address = vt_socket_make_address(PORT, IP_ADDRESS);
    const vt_socket_t server_id = vt_socket_startup_client(VT_SOCKET_TYPE_TCP, server_address);

    while (true) {
        const char *msg = "Hello! - from client.";
        const size_t msg_len = strlen(msg);

        // send
        const int32_t size_sent = vt_socket_send(server_id, msg, msg_len);

        // receive
        static char buffer[VT_STR_TMP_BUFFER_SIZE] = {0};
        const int32_t size_received = vt_socket_receive(server_id, buffer, VT_STR_TMP_BUFFER_SIZE-1);
        if (size_received > 0) buffer[size_received] = '\0';
    }
    
    // close connection
    vt_socket_close(server_id);
}
assert(vt_socket_quit() == true);
```

**[ [Back](page9.md) | [Contents](VITA.md) ]**

