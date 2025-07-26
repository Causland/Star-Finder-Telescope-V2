#ifndef __NETWORK_CONSTANTS_H__
#define __NETWORK_CONSTANTS_H__

#include <cstdint>

static constexpr const char* WIFI_GATEWAY_ADDR{"192.168.4.1"};
static constexpr const char* WIFI_TELESCOPE_ADDR{"192.168.4.1"};
static constexpr const char* WIFI_USER_ADDR{"192.168.4.2"};
static constexpr const char* WIFI_SUBNET{"255.255.255.0"};

static constexpr uint16_t WIFI_CMD_PORT{8000};
static constexpr uint16_t WIFI_TELEM_PORT{8001};
static constexpr uint16_t WIFI_CAMERA_PORT{8002};
static constexpr uint16_t WIFI_OTA_PORT{8003};
static constexpr uint16_t WIFI_LOG_PORT{8004};

#endif // __NETWORK_CONSTANTS_H__