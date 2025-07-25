#include <algorithm>
#include <chrono>
#include <cstring>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_ota_ops.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <thread>

#include "NetworkConstants.h"
#include "OTAUpdater.h"
#include "WifiInitializer.h"

extern const uint8_t SERVER_CERT_PEM_START[] asm("_binary_ca_cert_pem_start");
extern const uint8_t SERVER_CERT_PEM_END[] asm("_binary_ca_cert_pem_end");

static constexpr size_t DEFAULT_HTTP_CLIENT_TIMEOUT_MS{5000};

uint32_t OTAUpdater::appVersion{};

OTAUpdater::OTAUpdater()
{
  WifiInitializer::initialize();

  readAppVersion();

  config.cert_pem = reinterpret_cast<const char*>(SERVER_CERT_PEM_START);
  config.timeout_ms = 5000;
  config.keep_alive_enable = true;

  otaConfig.http_config = &config;
}

bool OTAUpdater::performUpdate(const std::string& filename)
{
  if (!WifiInitializer::isClientConnected())
  {
    ESP_LOGE(TAG, "Client is not connected for OTA update");
    return false;
  }

  if (setupUpdate(filename) != ESP_OK)
  {
    return false;
  }

  if (esp_https_ota_begin(&otaConfig, &httpsOtaHandle) != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to start HTTPS OTA");
    return false;
  }

  esp_app_desc_t appDesc;
  if (esp_https_ota_get_img_desc(httpsOtaHandle, &appDesc) != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to obtain new app image description");
    return false;
  }

  if (validateImageHeader(appDesc) != ESP_OK)
  {
    ESP_LOGE(TAG, "Image header verification failed");
    return false;
  }

  esp_err_t err;
  while (true)
  {
    err = esp_https_ota_perform(httpsOtaHandle);
    
    if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) break;

    const auto len{esp_https_ota_get_image_len_read(httpsOtaHandle)};
    ESP_LOGD(TAG, "Read %d image bytes", len);
  }

  if (!esp_https_ota_is_complete_data_received(httpsOtaHandle))
  {
    ESP_LOGE(TAG, "Did not receive complete data for OTA");
    esp_https_ota_abort(httpsOtaHandle);
    return false;
  }

  const auto otaErr{esp_https_ota_finish(httpsOtaHandle)};
  if (err != ESP_OK || otaErr != ESP_OK)
  {
    if (otaErr == ESP_ERR_OTA_VALIDATE_FAILED)
    {
      ESP_LOGE(TAG, "Image validation failed. Image is corrupted");
    }
    ESP_LOGE(TAG, "HTTPS OTA upgrade failed. err=%d", otaErr);
    return false;
  }

  ESP_LOGI(TAG, "Successful HTTPS OTA upgrade. Rebooting...");
  shutdown(); // Expected no return
  return true; // Silence no return warnings
}

void OTAUpdater::httpsEventHandler(void* arg, esp_event_base_t eventBase, 
                                   int32_t eventId, void* eventData)
{
  if (eventBase == ESP_HTTPS_OTA_EVENT)
  {
    switch (eventId)
    {
      case ESP_HTTPS_OTA_START:
        ESP_LOGI(TAG, "OTA started");
        break;
      case ESP_HTTPS_OTA_CONNECTED:
        ESP_LOGI(TAG, "Connected to server");
        break;
      case ESP_HTTPS_OTA_GET_IMG_DESC:
        ESP_LOGI(TAG, "Reading Image Description");
        break;
      case ESP_HTTPS_OTA_VERIFY_CHIP_ID:
        ESP_LOGI(TAG, "Verifying chip id of new image: %d", 
                      *static_cast<esp_chip_id_t*>(eventData));
        break;
      case ESP_HTTPS_OTA_DECRYPT_CB:
        ESP_LOGI(TAG, "Callback to decrypt function");
        break;
      case ESP_HTTPS_OTA_WRITE_FLASH:
        ESP_LOGD(TAG, "Writing to flash: %d written", *static_cast<int*>(eventData));
        break;
      case ESP_HTTPS_OTA_UPDATE_BOOT_PARTITION:
        ESP_LOGI(TAG, "Boot partition updated. Next Partition: %d",
                      *static_cast<esp_partition_subtype_t*>(eventData));
        break;
      case ESP_HTTPS_OTA_FINISH:
        ESP_LOGI(TAG, "OTA finish");
        break;
      case ESP_HTTPS_OTA_ABORT:
        ESP_LOGI(TAG, "OTA abort");
        break;
    }
  }
}

esp_err_t OTAUpdater::setupUpdate(const std::string& filename)
{
  otaUrl = std::string("https://") + WIFI_USER_ADDR + ":" + 
        std::to_string(WIFI_OTA_PORT) + "/" + filename;
  config.url = otaUrl.c_str();
  ESP_LOGI(TAG, "Updating via HTTPS from %s", config.url);

  esp_err_t err{nvs_flash_init()};
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    err = nvs_flash_erase();
    if (err != ESP_OK)
    {
      ESP_LOGE(TAG, "Failed to erase flash. Not enough space for OTA");
      return err;
    }

    err = nvs_flash_init();
    if (err != ESP_OK)
    {
      ESP_LOGE(TAG, "Failed to init flash: %d", err);
      return err;
    }
  }

  return ESP_OK;
}

esp_err_t OTAUpdater::validateImageHeader(const esp_app_desc_t& newAppInfo)
{
  const auto* runningPart{esp_ota_get_running_partition()};
  esp_app_desc_t runningAppInfo;
  if (esp_ota_get_partition_description(runningPart, &runningAppInfo) == ESP_OK)
  {
    ESP_LOGI(TAG, "Running firmware version: %s", runningAppInfo.version);
  }

  if (std::strncmp(newAppInfo.version, runningAppInfo.version, sizeof(newAppInfo.version)) == 0)
  {
    ESP_LOGW(TAG, "Current version is same as new version. Stopping update");
    return ESP_FAIL;
  }

  return ESP_OK;
}

void OTAUpdater::shutdown()
{ 
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  esp_restart();
}


void OTAUpdater::readAppVersion()
{
  esp_app_desc_t appDesc;
  esp_ota_get_partition_description(esp_ota_get_running_partition(), &appDesc);

  uint32_t maj{};
  uint32_t min{};
  uint32_t pat{};
  std::sscanf(appDesc.version, "v%lu.%lu.%lu", &maj, &min, &pat);

  appVersion = (((maj & 0xFF) << 16) | ((min & 0xFF) << 8) | (pat & 0xFF));
}