#include "GPS.h"
#include "Telemetry.h"
#include "Utils.h"

bool GPS::init()
{
  Serial1.begin(9600);
  return true;
}

bool GPS::getData()
{
  int retries = 5; // 30 seconds to get a fix
  DEBUG_PRINT("Waiting for GPS fix...");
  while (retries > 0)
  {
    while (Serial1.available())
    {
      // Read the GPS data
      gps.encode(Serial1.read()); 
    }

    if (gps.location.isValid())
    {
      gpsLat = gps.location.lat();
      gpsLon = gps.location.lng();
      gpsAlt = gps.altitude.feet();
      DEBUG_PRINTLN(" ");
      return true;
    }
    else
    {
      // Wait for a short period before retrying. Note this is
      // not a FreeRTOS call because this is running in setup()
      delay(1000);
#ifdef DEBUG
      Serial.print(".");
#endif
      --retries;
    }
  }
  DEBUG_PRINTLN(" ");
  return false;
}

void GPS::deinit()
{
  Serial1.end();
}