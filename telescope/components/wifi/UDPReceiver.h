#ifndef __UDP_RECEIVER_H__
#define __UDP_RECEIVER_H__

#include <cstdint>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>

/// UDPReceiver handles receiving UDP packets on the specified local address and port.
/// It is designed to be used in a single-threaded context, where it can receive data
/// from a UDP socket and process it as needed.
class UDPReceiver
{
public:
  /// Constructs a UDPReceiver object with the specified name, local address, and port.
  ///
  /// @param[in] name The name of the receiver, used for logging.
  /// @param[in] localAddr The local address to bind this socket to.
  /// @param[in] localPort The local port to bind this socket to.
  UDPReceiver(const std::string& name, const std::string& localAddr, const uint16_t& localPort);
  
  UDPReceiver(const UDPReceiver&) = delete;
  UDPReceiver(UDPReceiver&&) = delete;

  /// Stops the receiver by closing the socket.
  ~UDPReceiver();

  UDPReceiver& operator=(const UDPReceiver&) = delete;
  UDPReceiver& operator=(UDPReceiver&&) = delete;

  /// Stops the receiver and closes the socket.
  void stop();

  /// Receives data from the UDP socket. This function blocks
  /// the calling thread until data is available or the socket is closed.
  ssize_t receive(uint8_t* data, const size_t& dataLen) const;

private:
  std::string name; ///< Name of the receiver, used for logging
  struct sockaddr_in sockAddr; ///< Socket address structure for binding
  int sockfd{-1}; ///< Socket file descriptor
};

#endif