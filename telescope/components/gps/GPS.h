#ifndef __GPS_H__
#define __GPS_H__

#include <TinyGPS++.h>

#include "gps_secrets.h"

class GPS
{
public:
  GPS();

  GPS(const GPS&) = delete;
  GPS(GPS&&) = delete;

  ~GPS();

  GPS& operator=(const GPS&) = delete;
  GPS& operator=(GPS&&) = delete;

  /// Update the GPS data
  ///
  /// @return true if the GPS data was updated successfully, false otherwise
  bool getData();

  float gpsLat{DEFAULT_GPS_LAT};
  float gpsLon{DEFAULT_GPS_LON};
  float gpsAlt{DEFAULT_GPS_ALT};

private:
  TinyGPSPlus gps; ///< GPS object to read data from the GPS module
};

#endif