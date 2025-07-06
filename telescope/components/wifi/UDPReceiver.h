#ifndef __UDP_RECEIVER_H__
#define __UDP_RECEIVER_H__

#include <cstdint>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>

class UDPReceiver
{
public:
    UDPReceiver(const std::string& name, const std::string& localAddr, const uint16_t& localPort);
    ~UDPReceiver();
    
    UDPReceiver(const UDPReceiver&) = delete;
    UDPReceiver(UDPReceiver&&) = delete;
    UDPReceiver& operator=(const UDPReceiver&) = delete;
    UDPReceiver& operator=(UDPReceiver&&) = delete;

    void stop();

    ssize_t receive(uint8_t* data, const size_t& dataLen);

private:
    std::string name;
    struct sockaddr_in sockAddr;
    int sockfd{-1}; ///< Socket file descriptor
};

#endif