#ifndef __WIFI_H__
#define __WIFI_H__

#include <esp_wifi.h>

const IPAddress WIFI_GATEWAY{192, 168, 1, 1};
const IPAddress WIFI_TELESCOPE_ADDR{192, 168, 1, 2};
const IPAddress WIFI_USER_ADDR{192, 168, 1, 3};
const IPAddress WIFI_SUBNET{255, 255, 255, 0};

static constexpr uint16_t WIFI_CMD_PORT{8000};
static constexpr uint16_t WIFI_TELEM_PORT{8001};
static constexpr uint16_t WIFI_CAMERA_PORT{8002};

struct WiFiWrapper
{
  /// Constructor
  WiFiWrapper() = default;

  /// Destructor
  ~WiFiWrapper() { stop(); };

  WiFiWrapper(const WiFiWrapper&) = delete;
  WiFiWrapper(WiFiWrapper&&) = delete;
  WiFiWrapper& operator=(const WiFiWrapper&) = delete;
  WiFiWrapper& operator=(WiFiWrapper&&) = delete;

  /// Start a WiFi access point with the SSID and password defined in secrets.h.
  /// This AP wil be used to command the telescope and receive telemetry data.
  /// Additionally, configure UDP receivers and senders for commands and telemetry data
  /// 
  /// @return true if the AP was started successfully, false otherwise
  bool init();

  /// Stop the WiFi access point and free up resources
  void stop();

  WiFiUDP cmdReceiver; ///< Used to receive commands
  WiFiUDP telemSender; ///< Used to send telemetry data
  WiFiUDP cameraSender; ///< Used to send camera data
};

#endif