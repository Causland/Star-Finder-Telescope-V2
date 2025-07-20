#ifndef __WIFI_INITIALIZER_H__
#define __WIFI_INITIALIZER_H__

#include <atomic>
#include <esp_event.h>
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

  /// @return true if there is a client connected to the AP
  static bool isClientConnected() { return clientConnected; }

  WifiInitializer(const WifiInitializer&) = delete;
  WifiInitializer(WifiInitializer&&) = delete;

  /// Destructor cleans up the Wifi subsystem.
  ~WifiInitializer();

  WifiInitializer& operator=(const WifiInitializer&) = delete;
  WifiInitializer& operator=(WifiInitializer&&) = delete;

private:
  WifiInitializer() = default;

  /// Called when a new client connects to the AP. Sets the client connected flag to
  /// allow senders to send data
  static void wifiAPConnectHandler(void* handlerArg, esp_event_base_t base,
                                   int32_t id, void* eventData);

  /// Called when a client disconnects from the AP. Prevents senders from sending
  /// data when no clients are connected. 
  static void wifiAPDisconnectHandler(void* handlerArg, esp_event_base_t base,
                                      int32_t id, void* eventData);

  /// Internal function to perform the actual WiFi initialization.
  /// This function can only be called once due to the use of std::once_flag.
  static void initWifiOnce();

  static std::atomic<bool> clientConnected; ///< True if a client is currently connected.
                                            ///< Prevents send attempts when there is no
                                            ///< client connected to the AP

  static std::once_flag initFlag; ///< Flag to ensure WiFi is initialized only once

  static esp_netif_t* netif; ///< Pointer to the WiFi network interface
  static esp_event_handler_instance_t connectInstance; ///< Handler for controlling connect
                                                       ///< event. Used for cleanup
  static esp_event_handler_instance_t disconnectInstance; ///< Handler for controlling disconnect
                                                          ///< event. Used for cleanup

  static constexpr const char* TAG{"WifiInitializer"}; ///< Tag for logging purposes
};

#endif // __WIFI_INITIALIZER_H__