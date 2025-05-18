#ifndef __GPS_H__
#define __GPS_H__

#include <TinyGPSPlus.h>

#include "secrets.h"

struct GPS
{
  GPS() = default;

  /// Initialize the GPS module serial port
  ///
  /// @return true if the GPS module was initialized successfully, false otherwise
  bool init();

  /// Deinitialize the GPS module serial port
  void deinit();

  /// Update the GPS data
  ///
  /// @return true if the GPS data was updated successfully, false otherwise
  bool getData();

  float gpsLat{DEFAULT_GPS_LAT};
  float gpsLon{DEFAULT_GPS_LON};
  float gpsAlt{DEFAULT_GPS_ALT};

  TinyGPSPlus gps; ///< GPS object to read data from the GPS module
};

#endif