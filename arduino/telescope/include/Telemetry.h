#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#define TELEM_FIELD(name, type)                    \
public:                                            \
  void registerTelemField##name(type* const field) \
  {                                                \
    if (field == nullptr) return;                  \
    name = field;                                  \
  }                                                \
private:                                           \
  type* name{nullptr};

class Telemetry
{
public:
  /// Serialize the telemetry data into a buffer
  /// The buffer must be large enough to hold all the telemetry data
  ///
  /// @param[in,out] buffer The buffer to hold the serialized data
  /// @param[in] bufferSize The size of the buffer
  /// @return bytes serialized, -1 if buffer is too small
  int serializeTelemetry(uint8_t* buffer, const size_t& bufferSize);

  // General Info (Not from other tasks)
  // These fields are intentionally commented out to show they
  // will be serialized in this order

  // TELEM_FIELD(SystemTime, unsigned long);

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