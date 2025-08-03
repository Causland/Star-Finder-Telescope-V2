#ifndef __OTA_UPDATER_H__
#define __OTA_UPDATER_H__

#include <esp_http_client.h>
#include <esp_https_ota.h>
#include <string>

/// Provides functionality to perform OTA updates on the telescope using HTTPS.
class OTAUpdater
{
public:
  /// Constructs an OTAUpdater object, initializing the WiFi, and 
  /// setting up default HTTPS OTA configurations.
  OTAUpdater();

  /// Performs an OTA update using the specified filename. If successful,
  /// the device will reboot to apply the update.
  ///
  /// @param[in] filename the name of the file to update from on the server.
  ///
  /// @return true if the update was successful, false otherwise.
  ///         This function will never return if the update is successful.
  bool performUpdate(const std::string& filename);

  /// @return the current app version packed into a uint32_t.
  /// @see readAppVersion()
  static uint32_t getAppVersion() { return appVersion; }

private:
  /// Event handler for HTTPS OTA events.
  ///
  /// @param[in] arg user-defined argument passed to the event handler
  /// @param[in] eventBase the base event type
  /// @param[in] eventId the specific event ID
  /// @param[in] eventData additional data associated with the event
  static void httpsEventHandler(void* arg, esp_event_base_t eventBase, 
                                int32_t eventId, void* eventData);

  /// Sets up the OTA update with the specified filename and initializes the NVS.
  ///
  /// @param[in] filename the name of the file to update from on the server.
  ///
  /// @return ESP_OK if setup was successful, an error code otherwise.
  esp_err_t setupUpdate(const std::string& filename);

  /// Validates the image header of the new application to ensure it is not the same
  /// as the currently running application.
  ///
  /// @param[in] newAppInfo the application description of the new image
  ///
  /// @return ESP_OK if the image header is valid, an error code otherwise.
  esp_err_t validateImageHeader(const esp_app_desc_t& newAppInfo);

  /// Reboots the device to apply the OTA update.
  [[noreturn]] void shutdown();
  
  /// Reads the current application version from the running partition.
  /// Packs the version into a uint32_t in the format (0x00, 0xMaj, 0xMin, 0xPatch).
  static void readAppVersion();

  esp_http_client_config_t config{}; ///< Configuration for the HTTPS client used for OTA updates
  esp_https_ota_config_t otaConfig{}; ///< Configuration for the HTTPS OTA process
  esp_https_ota_handle_t httpsOtaHandle{}; ///< Handle for the ongoing OTA process

  std::string otaUrl; ///< URL for the OTA update file on the server. (e.g., https://<IP>:<port>/<filename>)
  
  static uint32_t appVersion; ///< Holds the version byte representation (0x00, 0xMaj, 0xMin, 0xPatch)

  static constexpr const char* TAG{"OTAUpdater"}; ///< The tag for logging
};

#endif