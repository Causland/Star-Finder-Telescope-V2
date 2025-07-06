#ifndef __WIFI_INITIALIZER_H__
#define __WIFI_INITIALIZER_H__

#include <esp_netif.h>
#include <mutex>

/// Static class to initialize the Wifi subsystem of the ESP32.
/// This class ensures that the WiFi is initialized only once, even if multiple components
/// attempt to intialize it concurrently.
class WifiInitializer
{
public:
  /// Initializes the Wifi subsystem.
  /// This function must be called before any other WiFi-related operations.
  static void initialize();

  WifiInitializer(const WifiInitializer&) = delete;
  WifiInitializer(WifiInitializer&&) = delete;

  /// Destructor cleans up the Wifi subsystem.
  ~WifiInitializer();

  WifiInitializer& operator=(const WifiInitializer&) = delete;
  WifiInitializer& operator=(WifiInitializer&&) = delete;

private:
  WifiInitializer() = default;

  /// Internal function to perform the actual WiFi initialization.
  /// This function can only be called once due to the use of std::once_flag.
  static void initWifiOnce();

  static std::once_flag initFlag; ///< Flag to ensure WiFi is initialized only once

  static esp_netif_t* netif; ///< Pointer to the WiFi network interface

  static constexpr const char* TAG{"WifiInitializer"}; ///< Tag for logging purposes
};

#endif // __WIFI_INITIALIZER_H__