#include <esp_heap_caps.h>
#include <esp_log.h>
#include <esp_timer.h>

#include "Telemetry.h"

#define SERIALIZE_FIELD(name)                                                             \
  if (name##Func)                                                                         \
  {                                                                                       \
    if (!serialize(name##Func()))                                                         \
    {                                                                                     \
      ESP_LOGE(pcTaskGetName(NULL), "Telemetry field " #name " does not fit in buffer!"); \
      return -1;                                                                          \
    }                                                                                     \
                                                                                          \
    offset += sizeof(*name);                                                              \
  }                                                                                       \
  else                                                                                    \
  {                                                                                       \
    ESP_LOGE("Telemetry field " #name " is not callable!");                               \
    return -1;                                                                            \
  }

Telemetry::Telemetry()
{
  // Register system fields
  registerTelemFieldSystemTimeCB(esp_timer_get_time);
  registerTelemFieldFreeHeapCB(std::bind(heap_caps_get_total_size, MALLOC_CAP_DEFAULT));
}

int Telemetry::serializeTelemetry(uint8_t* buffer, const size_t& bufferSize)
{
  if (buffer == nullptr) return false;

  // Serialize each field into the buffer
  size_t offset{0};

  SERIALIZE_FIELD(SystemTime);
  SERIALIZE_FIELD(FreeHeap);

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