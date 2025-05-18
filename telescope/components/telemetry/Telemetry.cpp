#include "Commands.h"
#include "Telemetry.h"
#include "Utils.h"

#define SERIALIZE_FIELD(name)                                                     \
  if (name != nullptr)                                                            \
  {                                                                               \
    if (!serialize(*name, buffer + offset, bufferSize - offset))                  \
    {                                                                             \
      DEBUG_TELEMETRY_PRINT("Telemetry field " #name " does not fit in buffer!"); \
      return -1;                                                                  \
    }                                                                             \
                                                                                  \
    offset += sizeof(*name);                                                      \
  }                                                                               \
  else                                                                            \
  {                                                                               \
    DEBUG_TELEMETRY_PRINT("Telemetry field " #name " is null!");                  \
    return -1;                                                                    \
  }

int Telemetry::serializeTelemetry(uint8_t* buffer, const size_t& bufferSize)
{
  if (buffer == nullptr) return false;

  // Serialize each field into the buffer
  size_t offset{0};

  const unsigned long timeNow = millis();
  SERIALIZE_FIELD(&timeNow);
  const size_t freeHeap = xPortGetFreeHeapSize();
  SERIALIZE_FIELD(&freeHeap);

  SERIALIZE_FIELD(CmdsReceived);

  SERIALIZE_FIELD(GPSLat);
  SERIALIZE_FIELD(GPSLon);
  SERIALIZE_FIELD(GPSAlt);

  SERIALIZE_FIELD(TrajRunning);
  SERIALIZE_FIELD(TrajNumEntries);
  SERIALIZE_FIELD(TrajCurrEntry);
  SERIALIZE_FIELD(TimeToNextEntry);

  SERIALIZE_FIELD(CurrAz);
  SERIALIZE_FIELD(CurrEl);
  SERIALIZE_FIELD(SpeedAz);
  SERIALIZE_FIELD(TargetAz);
  SERIALIZE_FIELD(TargetEl);

  return offset;
}