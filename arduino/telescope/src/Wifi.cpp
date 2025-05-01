#include <WiFiS3.h>

#include "arduino_secrets.h"
#include "Utils.h"
#include "Wifi.h"

bool Wifi::init()
{
  DEBUG_ENTER("Wifi::init()");

  // Configure the WiFi access point
  ap.config(WIFI_CMD_ADDR, WIFI_DNS, WIFI_GATEWAY, WIFI_SUBNET);

  // Start the WiFi access point
  if (ap.beginAP(SECRET_SSID, SECRET_PASS) != WL_AP_LISTENING)
  {
    DEBUG_PRINTLN("Failed to start WiFi access point!");
    return false;
  }
  DEBUG_PRINTLN("WiFi access point started successfully!");

  // Print the IP address of the access point
  DEBUG_PRINTLN("IP address: " + ap.localIP().toString());

  // Start the UDP receiver for commands
  if (!cmdReceiver.begin(WIFI_CMD_ADDR, WIFI_CMD_PORT))
  {
    DEBUG_PRINTLN("Failed to start command receiver!");
    return false;
  }

  // Start the UDP sender for telemetry
  if (!telemSender.begin(WIFI_TELEM_ADDR, WIFI_TELEM_PORT))
  {
    DEBUG_PRINTLN("Failed to start telemetry sender!");
    return false;
  }

  DEBUG_EXIT("Wifi::init()");
  return true;
}

void Wifi::stop()
{
  DEBUG_ENTER("Wifi::stop()");

  // Stop the WiFi access point
  ap.end();

  // Stop the UDP receiver and sender
  cmdReceiver.stop();
  telemSender.stop();

  DEBUG_EXIT("Wifi::stop()");
}