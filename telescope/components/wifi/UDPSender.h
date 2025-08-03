#ifndef __UDP_SENDER_H__
#define __UDP_SENDER_H__

#include <cstdint>
#include <esp_check.h>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>

/// UDPSender handles sending UDP packets to a specified destination address and port.
class UDPSender
{
public:
  /// Constructs a UDPSender object with the specified name, destination address, and port.
  ///
  /// @param[in] name The name of the sender, used for logging.
  /// @param[in] destAddr The destination address to send packets to.
  /// @param[in] destPort The destination port to send packets to.
  UDPSender(const std::string& name, const std::string& destAddr, const uint16_t& destPort);

  UDPSender(const UDPSender&) = delete;
  UDPSender(UDPSender&&) = delete;

  /// Destructor that closes the socket if it is open.
  ~UDPSender();

  UDPSender& operator=(const UDPSender&) = delete;
  UDPSender& operator=(UDPSender&&) = delete;

  /// Send data to the destination specified during construction.
  ///
  /// @param[in] data Pointer to the data to send.
  /// @param[in] size The size of the data to send.
  ///
  /// @return ESP_OK if the data was sent successfully, an error code otherwise.
  esp_err_t send(const uint8_t* data, const size_t& size) const;

private:
  std::string name; ///< Name of the sender, used for logging
  struct sockaddr_in sockAddr; ///< Socket address structure for sending data
  int sockfd{-1}; ///< Socket file descriptor
};

#endif