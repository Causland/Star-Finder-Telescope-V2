#include "UDPReceiver.h"
#include "WifiInitializer.h"

#include <esp_log.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

UDPReceiver::UDPReceiver(const std::string& name,
                         const std::string& localAddr, const uint16_t& localPort)
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
    sockAddr.sin_port = htons(localPort);
    
    if (inet_pton(AF_INET, localAddr.c_str(), &sockAddr.sin_addr) <= 0)
    {
        ESP_LOGE(name.c_str(), "Invalid address/ Address not supported");
        stop();
    }

    if (bind(sockfd, reinterpret_cast<struct sockaddr*>(&sockAddr), sizeof(sockAddr)) < 0)
    {
        ESP_LOGE(name.c_str(), "Bind failed: %s", strerror(errno));
        stop();
    }
}

UDPReceiver::~UDPReceiver()
{
    stop();
}

void UDPReceiver::stop()
{
    if (sockfd >= 0)
    {
        close(sockfd);
        sockfd = -1;
    }
}

ssize_t UDPReceiver::receive(uint8_t* data, const size_t& dataLen)
{
    if (sockfd < 0)
    {
        ESP_LOGE(name.c_str(), "Socket is not initialized");
        return -1;
    }

    const ssize_t received = recvfrom(sockfd, data, dataLen, 0, nullptr, nullptr);

    ESP_LOGD(name.c_str(), "Received %zd bytes", received);
    return received;
}