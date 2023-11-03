#include "vita/network/sockets.h"

bool vt_socket_init(void) {
    #if defined(_WIN32) || defined(_WIN64)
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0) {
            VT_DEBUG_PRINTF("%s: Failed to initialize sockets!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
            return false;
        }
    #endif

    return true;
}

bool vt_socket_quit(void) {
    #if defined(_WIN32) || defined(_WIN64)
        if (WSACleanup() != 0) {
            VT_DEBUG_PRINTF("%s: Failed to quit sockets!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
            return false;
        }
    #endif

    return true;
}

struct VitaSocketAddress vt_socket_make_address(const int16_t port, const char *const address) {
    return (struct VitaSocketAddress) {
        .port = htons(port),
        .addr = inet_addr(address)
    };
}

vt_socket_t vt_socket_startup_server(const enum VitaSocketType type, const int32_t port, const int32_t backlog) {
    // create socket
    const vt_socket_t sock_fd = socket(AF_INET, type, 0);
    if(sock_fd == VT_SOCKET_STATUS_INVALID) {
        VT_DEBUG_PRINTF("%s: Failed to create a server socket!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_INVALID;
    }

    // set socket options
    const int32_t opt = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(int)) < 0) {
        VT_DEBUG_PRINTF("%s: Failed to set socket options!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_ERROR_OPTIONS;
    }

    // create server socket address
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // forcefully bind the server socket address
    if (bind(sock_fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        VT_DEBUG_PRINTF("%s: Failed to bind the socket!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_ERROR_BIND;
    }

    // listen
    if (type == SOCK_STREAM && listen(sock_fd , backlog) < 0) {
        VT_DEBUG_PRINTF("%s: Error listening for connections!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_ERROR_LISTEN;
    }

    return sock_fd;
}

vt_socket_t vt_socket_startup_client(const enum VitaSocketType type, const struct VitaSocketAddress address) {
    // create socket
    const vt_socket_t sock_fd = socket(AF_INET, type, 0);
    if (sock_fd == VT_SOCKET_STATUS_INVALID) {
        VT_DEBUG_PRINTF("%s: Failed to create a client socket!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_INVALID;
    }

    // setup server address details
    struct sockaddr_in server = {0};
    server.sin_addr.s_addr = address.addr;
    server.sin_family = AF_INET;
    server.sin_port = address.port;

    // connect to remote server
    if (connect(sock_fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        VT_DEBUG_PRINTF("%s: Failed to connect to server!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_ERROR_CONNECT;
    }

    return sock_fd;
}

vt_socket_t vt_socket_accept_client(vt_socket_t sock_fd) {
    // prepare
    struct sockaddr_in client = {0};
    socklen_t sockaddr_len = sizeof(struct sockaddr_in);

    // accpet incoming connection
    const vt_socket_t client_sock_fd = accept(sock_fd , (struct sockaddr*)&client, &sockaddr_len);

    // check for errors
    if (client_sock_fd == VT_SOCKET_STATUS_INVALID) {
        VT_DEBUG_PRINTF("%s: Error accepting connections!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_INVALID;
    }

    return client_sock_fd;
}

bool vt_socket_close(const vt_socket_t sock_fd) {
    #if defined(_WIN32) || defined(_WIN64)
        if (closesocket(sock_fd) != 0) {
            VT_DEBUG_PRINTF("%s: Failed to close the socket!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
            return false;
        }
    #else
        if (close(sock_fd) != 0) {
            VT_DEBUG_PRINTF("%s: Failed to close the socket!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
            return false;
        }
    #endif

    return true;
}

int32_t vt_socket_send(const vt_socket_t sock_fd, const char *const data_buf, const size_t data_len) {
    // send data
    const int32_t size_sent = send(sock_fd, data_buf, data_len, 0);

    // check for errors
    if (size_sent < 0) {
        VT_DEBUG_PRINTF("%s: Error sending data packet!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_ERROR_SEND;
    }
    
    return size_sent;
}

int32_t vt_socket_send_to(const vt_socket_t sock_fd, const struct VitaSocketAddress address, const char *const data_buf, const size_t data_len) {
    // setup destination socket address
    struct sockaddr_in dst_addr = {0};  
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = address.addr;
    dst_addr.sin_port = address.port;

    // send data
    const int32_t size_sent = sendto(sock_fd, data_buf, data_len, 0, (struct sockaddr*)&dst_addr, sizeof(dst_addr));
    
    // check for errors
    if (size_sent < 0) {
        VT_DEBUG_PRINTF("%s: Error sending data packet!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_ERROR_SEND;
    }
    
    return size_sent;
}

int32_t vt_socket_receive(const vt_socket_t sock_fd, char *const data_buf, const size_t data_len) {
    // receive data
    const int32_t size_received = recv(sock_fd, data_buf, data_len, 0);

    // check for errors
    if (size_received < 0) {
        VT_DEBUG_PRINTF("%s: Error receiving data packet!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_ERROR_RECEIVE;
    }
    
    return size_received;
}

int32_t vt_socket_receive_from(const vt_socket_t sock_fd, struct VitaSocketAddress *const address, char *data_buf, const size_t data_len) {
    // setup source socket address of incoming packets
    struct sockaddr_in src_addr = {0};
    socklen_t addrlen = sizeof(src_addr);
    
    // receive data
    const int32_t size_received = recvfrom(sock_fd, data_buf, data_len, 0, (struct sockaddr*)&src_addr, &addrlen);
    
    // check for errors
    if (size_received < 0) {
        VT_DEBUG_PRINTF("%s: Error receiving data packet!\n", vt_status_to_str(VT_STATUS_OPERATION_FAILURE));
        return VT_SOCKET_STATUS_ERROR_RECEIVE;
    }

    // update the source's address details
    address->port = src_addr.sin_port;
    address->addr = src_addr.sin_addr.s_addr;
    
    return size_received;
}

int32_t vt_socket_poll(struct pollfd *const pfd, const size_t pfd_size, const uint32_t timeout) {
    int32_t ret = 0;

    // poll events
    #if defined(_WIN32) || defined(_WIN64)
        ret = WSAPoll(pfd, (unsigned long)pfd_size, timeout);
    #else
        ret = poll(pfd, (nfds_t)pfd_size, timeout);
    #endif

    return (ret < 0) ? VT_SOCKET_STATUS_ERROR_POLL : ret;
}

int32_t vt_socket_receive_timed(const vt_socket_t sock_fd, char *data_buf, const size_t data_len, const uint32_t timeout) {
    // create poll file descriptor
    struct pollfd pfd = {
        .fd = sock_fd, 
        .events = POLLIN
    };

    // poll events
    const int32_t poll_ret = vt_socket_poll(&pfd, 1, timeout);
    if (poll_ret < 0) return VT_SOCKET_STATUS_ERROR_POLL;

    // receive data
    if (poll_ret > 0 && (pfd.revents & POLLIN)) {
        return vt_socket_receive(sock_fd, data_buf, data_len);
    }

    return 0;
}

int32_t vt_socket_receive_timed_from(const vt_socket_t sock_fd, struct VitaSocketAddress *const address, char *data_buf, const size_t data_len, const uint32_t timeout) {
    // create poll file descriptor
    struct pollfd pfd = {
        .fd = sock_fd, 
        .events = POLLIN
    };

    // poll events
    const int32_t poll_ret = vt_socket_poll(&pfd, 1, timeout);
    if (poll_ret < 0) return VT_SOCKET_STATUS_ERROR_POLL;

    // receive data
    if (poll_ret > 0 && (pfd.revents & POLLIN)) {
        return vt_socket_receive_from(sock_fd, address, data_buf, data_len);
    }

    return 0;
}

