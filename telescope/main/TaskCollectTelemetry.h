#ifndef __TASK_COLLECT_TELEMETRY_H__
#define __TASK_COLLECT_TELEMETRY_H__

#include <chrono>

#include "CustomTask.h"
#include "NetworkConstants.h"
#include "Telemetry.h"
#include "UDPSender.h"

class TaskCollectTelemetry : public CustomTask
{
public:
  TaskCollectTelemetry(const esp_pthread_cfg_t& threadConfig)
                        : CustomTask(threadConfig) {}

  TaskCollectTelemetry(const TaskCollectTelemetry&) = delete;
  TaskCollectTelemetry(TaskCollectTelemetry&&) = delete;

  ~TaskCollectTelemetry() override = default;

  TaskCollectTelemetry& operator=(const TaskCollectTelemetry&) = delete;
  TaskCollectTelemetry& operator=(TaskCollectTelemetry&&) = delete;

  /// Get the telemetry object that will be used to collect data. This function
  /// is used to access the object to register data callbacks.
  Telemetry& getTelemetry() { return telemetry; }

protected:
  void threadLoop() override;

private:
  Telemetry telemetry; ///< Telemetry object to collect data
  UDPSender telemSender{"TelemSender", WIFI_USER_ADDR, WIFI_TELEM_PORT}; ///< UDP sender for telemetry data
  std::chrono::milliseconds telemRate{250}; ///< Telemetry rate in milliseconds

  static constexpr uint16_t MIN_TELEM_RATE_MS{100}; ///< Minimum telemetry rate in milliseconds
};

#endif // __TASK_COLLECT_TELEMETRY_H__