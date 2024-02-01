#ifndef MROS_W24_SOLUTION_SERVER_SOCKET_HPP
#define MROS_W24_SOLUTION_SERVER_SOCKET_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstring>
#include <memory>
#include <optional>
#include <socket/connection_socket.hpp>
#include <socket/message_socket/connection_message_socket.hpp>
#include <socket/socket.hpp>
#include <socket/utils/socket_exception.hpp>
#include <string>
#include <type_traits>

/**
 * Class wrapping server sockets. Accepts client connect() calls and returns connected sockets, which should be passed
 * as the template argument.
 */
class ServerSocket : public Socket {
 public:
  /**
   * Initialize a server socket. Calls socket() bind() and listen(), the last of which will begin to store pending
   * connections in the socket's kernel space backlog. Implements a nonblocking socket.
   * @param domain The communication domain code to be used. Supports only AF_INET (IPv4).
   * @param address The address to bind to in x.x.x.x format.
   * @param port The port number to bind to.
   * @param listen_backlog The number of pending connections to allow before dropping new connections.
   * @throws SocketException Throws exception on failure of socket(), bind(), or listen().
   */
  ServerSocket(int const domain, const std::string &address, int port, int const listen_backlog);

  /**
   * Initialize a server socket with unknown address information. Generates address information and
   * calls the above constructor with the full set of argument
   * @param domain The communication domain code to be used. Currently supports only AF_INET (IPv4).
   * @param listen_backlog listen_backlog The number of pending connections to allow before dropping new connections.
   * @throws SocketException Throws exception on failure of socket(), bind(), or listen().
   */
  ServerSocket(int domain, int listen_backlog);

  /**
   * Close socket file descriptor on destruction.
   */
  ~ServerSocket() override;

  /**
   * Accept the first connection in the listening backlog and generate a connected socket of type ConnectionType.
   * Subclasses should template with the associated ConnectionSocket type. These must return an error
   * @return Shared pointer to generated connection socket class instance.
   * @throws SocketException Throws exception on failure to accept.
   */
  template <class ConnectionSocket>
  std::shared_ptr<ConnectionSocket> acceptConnection();

  /**
   * Close the socket's file descriptor if it is not already closed.
   */
  void close();

 protected:
  /**
   * Struct to store server address.
   */
  sockaddr_in server_address_;

  /**
   * Boolean, true if socket file descriptor is open, false otherwise.
   */
  bool is_open_ = false;

  /**
   * Helper function to generate address and port
   * @param domain The communication domain code to be used. Currently supports only AF_INET (IPv4).
   * @return a pair of string, int with host and port respectively
   */
  static std::pair<std::string, int> getAddressInfo(int domain);
};

#endif  // MROS_W24_SOLUTION_SERVER_SOCKET_HPP
