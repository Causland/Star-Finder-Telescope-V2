#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#include <functional>

#include "Serializer.h"

#define TELEM_FIELD(name, type)                               \
public:                                                       \
  void registerTelemField##name##CB(std::function<type()> cb) \
  {                                                           \
    name##Func = std::move(cb);                               \
  }                                                           \
private:                                                      \
  std::function<type()> name##Func;


static constexpr std::size_t MAX_TELEM_LEN{256};

class Telemetry : public Serializer<MAX_TELEM_LEN>
{
public:
  Telemetry();

  /// Serialize the telemetry data into the telem buffer
  ///
  /// @return bytes serialized, -1 if buffer is too small
  int serializeTelemetry();

  // General Info (Not from other tasks)
  // These fields are intentionally commented out to show they
  // will be serialized in this order

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
};

#endif