#include <WiFi.h>
#include <WiFiAP.h>

#include "secrets.h"
#include "Utils.h"
#include "WiFiWrapper.h"

bool WiFiWrapper::init()
{
  DEBUG_ENTER("WiFiWrapper::init()");

  // Configure the WiFi access point
  if (!WiFi.softAPConfig(WIFI_TELESCOPE_ADDR, WIFI_GATEWAY, WIFI_SUBNET))
  {
    DEBUG_PRINTLN("Failed to configure WiFi access point!");
    return false;
  }

  // Start the WiFi access point
  if (!WiFi.softAP(SECRET_SSID, SECRET_PASS))
  {
    DEBUG_PRINTLN("Failed to start WiFi access point!");
    return false;
  }
  DEBUG_PRINTLN("WiFi access point started successfully!");

  // Print the IP address of the access point
  DEBUG_PRINTLN("IP address: " + WiFi.softAPIP().toString());

  // Start the UDP receiver for commands
  if (!cmdReceiver.begin(WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))
  {
    DEBUG_PRINTLN("Failed to start command receiver!");
    return false;
  }

  // Start the UDP sender for telemetry
  if (!telemSender.begin(WIFI_TELEM_PORT))
  {
    DEBUG_PRINTLN("Failed to start telemetry sender!");
    return false;
  }

  // Start the UDP sender for the camera
  if (!cameraSender.begin(WIFI_CAMERA_PORT))
  {
    DEBUG_PRINTLN("Failed to start camera sender!");
    return false;
  }

  DEBUG_EXIT("WiFiWrapper::init()");
  return true;
}

void WiFiWrapper::stop()
{
  DEBUG_ENTER("WiFiWrapper::stop()");

  // Stop the WiFi access point
  WiFi.softAPdisconnect(true);

  // Stop the UDP receiver and sender
  cmdReceiver.stop();
  telemSender.stop();

  DEBUG_EXIT("WiFiWrapper::stop()");
}