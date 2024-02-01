#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

/**
 * Abstract base class for socket.
 */
class Socket {
 protected:
  /**
   * Default constructor.
   */
  Socket();

  /**
   * Private constructor to be called by connection sockets.
   */
  explicit Socket(int file_descriptor);

  /**
   * Pure virtual destructor to make class abstract.
   */
  virtual ~Socket() = 0;

  /**
   * File descriptor for the socket.
   */
  int file_descriptor_;
};
