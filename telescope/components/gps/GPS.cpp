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

bool GPS::getData()
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
    gpsAlt = gps.altitude.meters();
    return true;
  }

  return false;
}