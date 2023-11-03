#ifndef VITA_NETWORK_SOCKETS_H
#define VITA_NETWORK_SOCKETS_H

/** SOCKETS MODULE
    - vt_socket_init
    - vt_socket_quit
    - vt_socket_make_address
    - vt_socket_startup_server
    - vt_socket_startup_client
    - vt_socket_accept_client
    - vt_socket_close
    - vt_socket_send
    - vt_socket_send_to
    - vt_socket_receive
    - vt_socket_receive_from
    - vt_socket_poll
    - vt_socket_receive_timed
    - vt_socket_receive_timed_from
*/

#include "vita/core/core.h"
#include "vita/util/debug.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

// custom socket type
typedef int64_t vt_socket_t;

// socket errors
#define VT_SOCKET_STATUS_SUCCESS        (0)                 // all good
#define VT_SOCKET_STATUS_INVALID        (vt_socket_t)(~0)   // invalid socket
#define VT_SOCKET_STATUS_ERROR_OPTIONS  (-1)                // failed to set options
#define VT_SOCKET_STATUS_ERROR_BIND     (-2)                // failed to bind
#define VT_SOCKET_STATUS_ERROR_LISTEN   (-3)                // failed to listen
#define VT_SOCKET_STATUS_ERROR_CONNECT  (-4)                // failed to connect
#define VT_SOCKET_STATUS_ERROR_SEND     (-5)                // failed to send
#define VT_SOCKET_STATUS_ERROR_RECEIVE  (-6)                // failed to receive
#define VT_SOCKET_STATUS_ERROR_POLL     (-7)                // failed to poll events

// socket protocol type
enum VitaSocketType {
    VT_SOCKET_TYPE_TCP = SOCK_STREAM,
    VT_SOCKET_TYPE_UDP = SOCK_DGRAM,
    VT_SOCKET_TYPE_COUNT = VT_SOCKET_TYPE_UDP
};

// socket address
struct VitaSocketAddress {
    uint16_t port;
    uint32_t addr;
};

/** Initialize sockets
    @returns `true` upon success, `false` upon failure
*/
extern bool vt_socket_init(void);

/** Quit sockets
    @returns `true` upon success, `false` upon failure
*/
extern bool vt_socket_quit(void);

/** Sets up address
    @param port port
    @param address ip address
    @returns struct VitaSocketAddress
*/
extern struct VitaSocketAddress vt_socket_make_address(const int16_t port, const char *const address);

/** Startup server socket
    @param type stream type
    @param port port to attach
    @param backlog qeue size
    @returns valid `vt_socket_t` upon success, `VT_SOCKET_STATUS_ERROR_*` on error
*/
extern vt_socket_t vt_socket_startup_server(const enum VitaSocketType type, const int32_t port, const int32_t backlog);

/** Startup client socket
    @param type stream type
    @param address connect address
    @returns valid `vt_socket_t` upon success, `VT_SOCKET_STATUS_ERROR_*` on error
*/
extern vt_socket_t vt_socket_startup_client(const enum VitaSocketType type, const struct VitaSocketAddress address);

/** Startup server socket
    @param sock_fd server socket file descriptor `vt_socket_t`
    @returns valid `vt_socket_t` upon success, `VT_SOCKET_STATUS_ERROR_*` on error
*/
extern vt_socket_t vt_socket_accept_client(const vt_socket_t sock_fd);

/** Closes socket connection
    @param sock_fd socket file descriptor `vt_socket_t`
    @returns `true` upon success, `false` upon failure
*/
extern bool vt_socket_close(const vt_socket_t sock_fd);

/** Sends data
    @param sock_fd socket file descriptor `vt_socket_t`
    @param data_buf data buffer
    @param data_len data size
    @returns `size_sent` upon success, `VT_SOCKET_STATUS_ERROR_SEND` upon failure
*/
extern int32_t vt_socket_send(const vt_socket_t sock_fd, const char *const data_buf, const size_t data_len);

/** Sends data to address
    @param sock_fd socket file descriptor `vt_socket_t`
    @param address destination address
    @param data_buf data buffer
    @param data_len data size
    @returns `size_sent` upon success, `VT_SOCKET_STATUS_ERROR_SEND` upon failure
*/
extern int32_t vt_socket_send_to(const vt_socket_t sock_fd, const struct VitaSocketAddress address, const char *const data_buf, const size_t data_len);

/** Receives data
    @param sock_fd socket file descriptor `vt_socket_t`
    @param data_buf data buffer
    @param data_len data size
    @returns `size_received` upon success, `VT_SOCKET_STATUS_ERROR_RECEIVE` upon failure
*/
extern int32_t vt_socket_receive(const vt_socket_t sock_fd, char *data_buf, const size_t data_len);

/** Receives data from address
    @param sock_fd socket file descriptor `vt_socket_t`
    @param address destination address
    @param data_buf data buffer
    @param data_len data size
    @returns `size_received` upon success, `VT_SOCKET_STATUS_ERROR_RECEIVE` upon failure
*/
extern int32_t vt_socket_receive_from(const vt_socket_t sock_fd, struct VitaSocketAddress *const address, char *data_buf, const size_t data_len);

/** Poll socket file descriptors
    @param pfd poll socket file descriptors array
    @param pfd_size array size
    @param timeout amount of time to delay in milliseconds
    @returns number of file descriptors with an updated event or `VT_SOCKET_STATUS_ERROR_POLL` upon failure
*/
extern int32_t vt_socket_poll(struct pollfd *const pfd, const size_t pfd_size, const uint32_t timeout);

/** Receives data from address with timeout
    @param sock_fd socket file descriptor `vt_socket_t`
    @param data_buf data buffer
    @param data_len data size
    @param timeout amount of time to delay in milliseconds
    @returns `size_received` upon success, `VT_SOCKET_STATUS_ERROR_RECEIVE` upon failure

    @note If `timeout==-1`, behaves like `vt_socket_receive_from`. 
    @note If `timeout==0`, immediately timeout.
*/
extern int32_t vt_socket_receive_timed(const vt_socket_t sock_fd, char *data_buf, const size_t data_len, const uint32_t timeout);

/** Receives data from address with timeout
    @param sock_fd socket file descriptor `vt_socket_t`
    @param address destination address
    @param data_buf data buffer
    @param data_len data size
    @param timeout amount of time to delay in milliseconds
    @returns `size_received` upon success, `VT_SOCKET_STATUS_ERROR_RECEIVE` upon failure

    @note If `timeout==-1`, behaves like `vt_socket_receive_from`. 
    @note If `timeout==0`, immediately timeout.
*/
extern int32_t vt_socket_receive_timed_from(const vt_socket_t sock_fd, struct VitaSocketAddress *const address, char *data_buf, const size_t data_len, const uint32_t timeout);

#endif // VITA_NETWORK_SOCKETS_H

