#include <arpa/inet.h>
#include <esp_check.h>
#include <esp_log.h>
#include <sys/socket.h>
#include <unistd.h>

#include "UDPSender.h"
#include "WifiInitializer.h"

UDPSender::UDPSender(const std::string& name,
                     const std::string& destAddr, const uint16_t& destPort)
                      : name(name)
{
  WifiInitializer::initialize();

  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (sockfd < 0)
  {
    ESP_LOGE(name.c_str(), "Socket creation failed");
    return;
  }

  memset(&sockAddr, 0, sizeof(sockAddr));
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_port = htons(destPort);
  
  if (inet_pton(AF_INET, destAddr.c_str(), &sockAddr.sin_addr) <= 0)
  {
    ESP_LOGE(name.c_str(), "Invalid address/ Address not supported");
    close(sockfd);
    sockfd = -1;
  }
}

UDPSender::~UDPSender()
{
  if (sockfd >= 0)
  {
    close(sockfd);
  }
}

esp_err_t UDPSender::send(const uint8_t* data, const size_t& size) const
{
  if (!WifiInitializer::isClientConnected())
  {
    return ESP_ERR_NOT_ALLOWED;
  }

  if (sockfd < 0)
  {
    return ESP_ERR_INVALID_STATE;
  }

  const ssize_t sentBytes = sendto(sockfd, data, size, 0,
                                   reinterpret_cast<const struct sockaddr*>(&sockAddr),
                                   sizeof(sockAddr));
  
  if (sentBytes < 0 || static_cast<size_t>(sentBytes) != size)
  {
    return ESP_FAIL;
  }

  return ESP_OK;
}