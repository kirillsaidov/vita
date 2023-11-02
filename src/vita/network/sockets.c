#include "vita/network/sockets.h"

bool vt_socket_init(void) {
    #if defined(_WIN32) || defined(_WIN64)
        WSADATA wsa;
        return (WSAStartup(MAKEWORD(2, 0), &wsa) == 0);
    #endif

    return true;
}

bool vt_socket_quit(void) {
    #if defined(_WIN32) || defined(_WIN64)
        return (WSACleanup() == 0);
    #endif

    return true;
}

struct VitaSocketAddress vt_socket_make_address(const int16_t port, const char *const address) {
    return (struct VitaSocketAddress) {
        .port = htons(port),
        .addr = inet_addr(address)
    };
}

vt_socket_t vt_socket_startup_server(const int32_t type, const int32_t port, const int32_t backlog) {
    // create socket
    const vt_socket_t sock_fd = socket(AF_INET, type, 0);
    if(sock_fd == VT_SOCKET_STATUS_INVALID) {
        return VT_SOCKET_STATUS_INVALID;
    }

    // set socket options
    const int32_t opt = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(int)) < 0) {
        return VT_SOCKET_STATUS_ERROR_OPTIONS;
    }

    // create server socket address
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // forcefully bind the server socket address
    if (bind(sock_fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        return VT_SOCKET_STATUS_ERROR_BIND;
    }

    // listen
    if (type == SOCK_STREAM && listen(sock_fd , backlog) < 0) return VT_SOCKET_STATUS_ERROR_LISTEN;

    return sock_fd;
}

vt_socket_t vt_socket_startup_client(const int32_t type, const struct VitaSocketAddress address) {
    // create socket
    const vt_socket_t sock_fd = socket(AF_INET, type, 0);
    if (sock_fd == VT_SOCKET_STATUS_INVALID) {
        return VT_SOCKET_STATUS_INVALID;
    }

    // setup server address details
    struct sockaddr_in server = {0};
    server.sin_addr.s_addr = address.addr;
    server.sin_family = AF_INET;
    server.sin_port = address.port;

    // connect to remote server
    if (connect(sock_fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        return VT_SOCKET_STATUS_ERROR_CONNECT;
    }

    return sock_fd;
}

vt_socket_t vt_socket_accept_client(vt_socket_t sock_fd) {
    // prepare
    struct sockaddr_in client = {0};
    int sockaddr_len = sizeof(struct sockaddr_in);

    // accpet incoming connection
    const vt_socket_t client_sock_fd = accept(sock_fd , (struct sockaddr*)&client, &sockaddr_len);

    // check for errors
    if (client_sock_fd == VT_SOCKET_STATUS_INVALID) {
        return VT_SOCKET_STATUS_INVALID;
    }

    return client_sock_fd;
}

bool vt_socket_close(const vt_socket_t sock_fd) {
    #if defined(_WIN32) || defined(_WIN64)
        return (closesocket(sock_fd) == 0);
    #else
        return (close(sock_fd) == 0);
    #endif
}

int64_t vt_socket_send(const vt_socket_t sock_fd, const char *const data_buf, const size_t data_len) {
    // send data
    const int64_t size_sent = send(sock_fd, data_buf, data_len, 0);

    // check for errors
    if (size_sent < 0) return VT_SOCKET_STATUS_ERROR_SEND;
    else return size_sent;
}

int64_t vt_socket_send_to(const vt_socket_t sock_fd, const struct VitaSocketAddress address, const char *const data_buf, const size_t data_len) {
    // setup destination socket address
    struct sockaddr_in dst_addr = {0};  
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = address.addr;
    dst_addr.sin_port = address.port;

    // send data
    const int64_t size_sent = sendto(sock_fd, data_buf, data_len, 0, (struct sockaddr*)&dst_addr, sizeof(dst_addr));
    
    // check for errors
    if (size_sent < 0) return VT_SOCKET_STATUS_ERROR_SEND;
    else return size_sent;
}

int64_t vt_socket_receive(const vt_socket_t sock_fd, char *const data_buf, const size_t data_len) {
    // receive data
    const int64_t size_received = recv(sock_fd, data_buf, data_len, 0);

    // check for errors
    if (size_received < 0) return VT_SOCKET_STATUS_ERROR_RECEIVE;
    else return size_received;
}

int64_t vt_socket_receive_from(const vt_socket_t sock_fd, struct VitaSocketAddress *const address, char *data_buf, const size_t data_len) {
    // setup source socket address of incoming packets
    struct sockaddr_in src_addr = {0};
    int addrlen = sizeof(src_addr);
    
    // receive data
    const int64_t size_received = recvfrom(sock_fd, data_buf, data_len, 0, (struct sockaddr*)&src_addr, &addrlen);
    
    // check for errors
    if (size_received < 0) return VT_SOCKET_STATUS_ERROR_RECEIVE;

    // update the source's address details
    address->port = src_addr.sin_port;
    address->addr = src_addr.sin_addr.s_addr;
    
    return size_received;
}

