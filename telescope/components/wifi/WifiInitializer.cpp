#include "WifiInitializer.h"
#include "wifi_secrets.h"
#include "NetworkConstants.h"

#include <cstring>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_netif.h>

std::atomic<bool> WifiInitializer::clientConnected{};
std::once_flag WifiInitializer::initFlag;
esp_netif_t* WifiInitializer::netif{nullptr};
esp_event_handler_instance_t WifiInitializer::connectInstance;
esp_event_handler_instance_t WifiInitializer::disconnectInstance;

void WifiInitializer::initialize()
{
  std::call_once(initFlag, WifiInitializer::initWifiOnce);
}

WifiInitializer::~WifiInitializer()
{
  esp_wifi_stop();
  esp_wifi_deinit();

  esp_wifi_clear_default_wifi_driver_and_handlers(netif);
  esp_netif_destroy(netif);
  esp_event_loop_delete_default();
  esp_event_handler_instance_unregister(WIFI_EVENT, WIFI_EVENT_AP_STACONNECTED,
                                        connectInstance);
  esp_event_handler_instance_unregister(WIFI_EVENT, WIFI_EVENT_AP_STADISCONNECTED,
                                        disconnectInstance);
  esp_netif_deinit();
}

void WifiInitializer::wifiAPConnectHandler(void* handlerArg, esp_event_base_t base,
                                           int32_t id, void* eventData)
{
  ESP_LOGI(TAG, "Client connection detected");
  WifiInitializer::clientConnected = true;
}

void WifiInitializer::wifiAPDisconnectHandler(void* handlerArg, esp_event_base_t base,
                                              int32_t id, void* eventData)
{
  ESP_LOGI(TAG, "Client disconnect detected");
  WifiInitializer::clientConnected = false;
}

void WifiInitializer::initWifiOnce()
{
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  netif = esp_netif_create_default_wifi_ap();

  esp_netif_ip_info_t ip_info;
  inet_pton(AF_INET, WIFI_TELESCOPE_ADDR, &ip_info.ip);
  inet_pton(AF_INET, WIFI_GATEWAY_ADDR, &ip_info.gw);
  inet_pton(AF_INET, WIFI_SUBNET, &ip_info.netmask);
  ESP_ERROR_CHECK(esp_netif_dhcps_stop(netif));  // Stop DHCP server before setting IP
  ESP_ERROR_CHECK(esp_netif_set_ip_info(netif, &ip_info));
  ESP_ERROR_CHECK(esp_netif_dhcps_start(netif));

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  cfg.nvs_enable = false; // Disable NVS to avoid using flash storage
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                      WIFI_EVENT_AP_STACONNECTED,
                                                      &WifiInitializer::wifiAPConnectHandler,
                                                      nullptr,
                                                      &connectInstance));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                      WIFI_EVENT_AP_STADISCONNECTED,
                                                      &WifiInitializer::wifiAPDisconnectHandler,
                                                      nullptr,
                                                      &disconnectInstance));

  wifi_config_t ap_config = {};
  strncpy(reinterpret_cast<char*>(ap_config.ap.ssid), SECRET_SSID, sizeof(ap_config.ap.ssid));
  strncpy(reinterpret_cast<char*>(ap_config.ap.password), SECRET_PASS, sizeof(ap_config.ap.password));
  ap_config.ap.ssid_len = 0;
  ap_config.ap.channel = 1;
  ap_config.ap.max_connection = 4;
  ap_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "Wi-Fi AP initialized: SSID=" SECRET_SSID);
}