#include <Arduino.h>

#include "GPS.h"

GPS::GPS()
{
  Serial2.begin(9600);
}

GPS::~GPS()
{
  Serial2.end();
}

bool GPS::findPosition()
{
  while (Serial2.available() > 0)
  {
    // Read the GPS data
    gps.encode(Serial2.read()); 
  }

  if (gps.location.isValid())
  {
    std::scoped_lock<std::mutex> lock(gpsMutex);
    gpsLat = gps.location.lat();
    gpsLon = gps.location.lng();
    gpsAlt = gps.altitude.meters();
    gpsInitialized = true;
    return true;
  }

  return false;
}

bool GPS::getPosition(float& lat, float& lon, float& alt) const
{
  std::scoped_lock<std::mutex> lock(gpsMutex);
  if (!gpsInitialized)
  {
    return false;
  }

  lat = gpsLat;
  lon = gpsLon;
  alt = gpsAlt;
  return true;
}