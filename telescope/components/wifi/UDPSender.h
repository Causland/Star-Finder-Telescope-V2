#ifndef __UDP_SENDER_H__
#define __UDP_SENDER_H__

#include <cstdint>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>

class UDPSender
{
public:
    UDPSender(const std::string& name, const std::string& destAddr, const uint16_t& destPort);
    ~UDPSender();

    UDPSender(const UDPSender&) = delete;
    UDPSender(UDPSender&&) = delete;
    UDPSender& operator=(const UDPSender&) = delete;
    UDPSender& operator=(UDPSender&&) = delete;

    bool send(const uint8_t* data, const size_t& size);

private:
    std::string name;
    struct sockaddr_in sockAddr;
    int sockfd{-1}; ///< Socket file descriptor
};

#endif