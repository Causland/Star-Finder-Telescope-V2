#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#include <functional>

#include "Serializer.h"

/// This macro defines a telemetry field with a callback to
/// retrieve the value.
#define TELEM_FIELD(name, type)                               \
public:                                                       \
  void registerTelemField##name##CB(std::function<type()> cb) \
  {                                                           \
    name##Func = std::move(cb);                               \
  }                                                           \
private:                                                      \
  std::function<type()> name##Func;


static constexpr std::size_t MAX_TELEM_LEN{256}; ///< Maximum length of the telemetry buffer

/// The telemetry class is responsible for collecting and serializing telemetry data
/// from various parts of the system. Each task can register a callback to provide
/// its telemetry data to this class, which will then be called when serializing the data.
class Telemetry : public Serializer<MAX_TELEM_LEN>
{
public:
  /// Constructs a Telemetry object and registers system telemetry fields.
  Telemetry();

  /// Serialize the telemetry data into the telem buffer
  ///
  /// @return bytes serialized, -1 if buffer is too small
  int serializeTelemetry();

  // General System Info
  TELEM_FIELD(SystemTime, unsigned long);
  TELEM_FIELD(FreeHeap, size_t);

  // Command Info
  TELEM_FIELD(CmdsReceived, uint16_t);

  // GPS Info
  TELEM_FIELD(GPSLat, float);
  TELEM_FIELD(GPSLon, float);
  TELEM_FIELD(GPSAlt, float);

  // Trajectory Info
  TELEM_FIELD(TrajRunning, bool);
  TELEM_FIELD(TrajNumEntries, uint8_t);
  TELEM_FIELD(TrajCurrEntry, uint8_t);
  TELEM_FIELD(TimeToNextEntry, unsigned long);

  // Servo Info
  TELEM_FIELD(CurrAz, double);
  TELEM_FIELD(CurrEl, double);
  TELEM_FIELD(SpeedAz, double);
  TELEM_FIELD(TargetAz, float);
  TELEM_FIELD(TargetEl, float);

private:
  static constexpr const char* TAG{"Telemetry"}; ///< The tag for logging
};

#endif