#ifndef __WIFI_H__
#define __WIFI_H__

#include <WiFiS3.h>

const IPAddress WIFI_GATEWAY{192, 168, 1, 1};
const IPAddress WIFI_CMD_ADDR{192, 168, 1, 2};
const IPAddress WIFI_TELEM_ADDR{192, 168, 1, 3};
const IPAddress WIFI_DNS{8, 8, 8, 8};
const IPAddress WIFI_SUBNET{255, 255, 255, 0};

static constexpr uint16_t WIFI_CMD_PORT{8000};
static constexpr uint16_t WIFI_TELEM_PORT{8001};

struct Wifi
{
  /// Constructor
  Wifi() = default;

  /// Destructor
  ~Wifi() { stop(); };

  Wifi(const Wifi&) = delete;
  Wifi(Wifi&&) = delete;
  Wifi& operator=(const Wifi&) = delete;
  Wifi& operator=(Wifi&&) = delete;

  /// Start a WiFi access point with the SSID and password defined in arduino_secrets.h.
  /// This AP wil be used to command the telescope and receive telemetry data.
  /// Additionally, configure UDP receivers and senders for commands and telemetry data
  /// 
  /// @return true if the AP was started successfully, false otherwise
  bool init();

  /// Stop the WiFi access point and free up resources
  void stop();

  CWifi ap; ///< Used to manage the AP
  WiFiUDP cmdReceiver; ///< Used to receive commands
  WiFiUDP telemSender; ///< Used to send telemetry data
};

#endif