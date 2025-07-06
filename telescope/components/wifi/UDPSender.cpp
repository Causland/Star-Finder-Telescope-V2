#include "UDPSender.h"
#include "WifiInitializer.h"

#include <esp_log.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

UDPSender::UDPSender(const std::string& name,
                     const std::string& destAddr, const uint16_t& destPort)
    : name(name)
{
    WifiInitializer::initialize(); // Ensure Wi-Fi is initialized

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

bool UDPSender::send(const uint8_t* data, const size_t& size)
{
    if (sockfd < 0)
    {
        ESP_LOGE(name.c_str(), "Socket is not initialized");
        return false;
    }

    const ssize_t sentBytes = sendto(sockfd, data, size, 0,
                                     reinterpret_cast<struct sockaddr*>(&sockAddr),
                                     sizeof(sockAddr));
    
    if (sentBytes < 0 || static_cast<size_t>(sentBytes) != size)
    {
        ESP_LOGE(name.c_str(), "Failed to send data: %s", strerror(errno));
        return false;
    }

    ESP_LOGD(name.c_str(), "Sent %zd bytes", sentBytes);
    return true;
}