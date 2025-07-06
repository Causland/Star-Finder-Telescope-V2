#include <Arduino.h>

#include "GPS.h"

GPS::GPS()
{
  Serial1.begin(9600);
}

GPS::~GPS()
{
  Serial1.end();
}

bool GPS::findPosition()
{
  while (Serial1.available())
  {
    // Read the GPS data
    gps.encode(Serial1.read()); 
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