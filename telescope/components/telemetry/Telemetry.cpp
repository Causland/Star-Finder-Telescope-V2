#include <esp_heap_caps.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "Telemetry.h"
#include "Serialization.h"

#define SERIALIZE_FIELD(name)                                                \
  if (name##Func)                                                            \
  {                                                                          \
    const auto result{Utils::serialize(telemBuffer.data() + bytesWritten,    \
                                       telemBuffer.size() - bytesWritten,    \
                                       name##Func())};                       \
    if (result < 0)                                                          \
    {                                                                        \
      ESP_LOGE(TAG, "Telemetry field " #name " does not fit in buffer!");    \
      return -1;                                                             \
    }                                                                        \
    bytesWritten += result;                                                  \
  }                                                                          \
  else                                                                       \
  {                                                                          \
    ESP_LOGW(TAG, "Telemetry field " #name " has no registered callback");   \
    bytesWritten += sizeof(decltype(name##Func)::result_type);               \
  }

Telemetry::Telemetry()
{
  // Register system fields
  registerTelemFieldSystemTimeCB(esp_timer_get_time);
  registerTelemFieldFreeHeapCB(std::bind(heap_caps_get_total_size, MALLOC_CAP_DEFAULT));
}

int Telemetry::serializeTelemetry()
{
  size_t bytesWritten{0};

  // Serialize each field into the buffer
  SERIALIZE_FIELD(Version);
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

  return bytesWritten;
}