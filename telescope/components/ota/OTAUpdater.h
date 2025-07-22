#ifndef __OTA_UPDATER_H__
#define __OTA_UPDATER_H__

#include <esp_http_client.h>
#include <esp_https_ota.h>
#include <string>

class OTAUpdater
{
public:
  OTAUpdater();

  bool performUpdate(const std::string& filename);

private:
  static void httpsEventHandler(void* arg, esp_event_base_t eventBase, 
                                int32_t eventId, void* eventData);

  esp_err_t setupUpdate(const std::string& filename);
  esp_err_t validateImageHeader(const esp_app_desc_t& newAppInfo);

  [[noreturn]] void shutdown();

  esp_http_client_config_t config{};
  esp_https_ota_config_t otaConfig{};
  esp_https_ota_handle_t httpsOtaHandle{};

  std::string otaUrl;

  static constexpr const char* TAG{"OTAUpdater"};
};

#endif