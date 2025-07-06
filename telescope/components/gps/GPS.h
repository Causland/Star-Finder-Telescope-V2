#ifndef __GPS_H__
#define __GPS_H__

#include <mutex>
#include <TinyGPS++.h>

#include "gps_secrets.h"

/// The GPS class provides an interface to interact with a GPS module
/// using the TinyGPS++ library over a serial connection.
class GPS
{
public:
  /// Starts serial communication with the GPS module and
  /// constructs the GPS object.
  GPS();

  GPS(const GPS&) = delete;
  GPS(GPS&&) = delete;

  /// Stops serial communication with the GPS module
  /// and destroys the GPS object.
  ~GPS();

  GPS& operator=(const GPS&) = delete;
  GPS& operator=(GPS&&) = delete;

  /// Attempt to read GPS data from the GPS module. Update the position
  /// if valid data is received.
  ///
  /// @return true if the GPS data was updated successfully, false otherwise
  bool findPosition();

  /// Get the current GPS position
  ///
  /// @param[out] lat Latitude in degrees
  /// @param[out] lon Longitude in degrees
  /// @param[out] alt Altitude in meters
  ///
  /// @return true if the GPS position is valid, false otherwise
  bool getPosition(float& lat, float& lon, float& alt) const;

private:
  TinyGPSPlus gps; ///< GPS object to decode data from the GPS module

  mutable std::mutex gpsMutex; ///< Mutex to protect access to GPS data
  
  bool gpsInitialized{false}; ///< Flag to indicate if GPS has been received

  float gpsLat{DEFAULT_GPS_LAT}; ///< Latitude of the GPS position
  float gpsLon{DEFAULT_GPS_LON}; ///< Longitude of the GPS position
  float gpsAlt{DEFAULT_GPS_ALT}; ///< Altitude of the GPS position in meters
};

#endif