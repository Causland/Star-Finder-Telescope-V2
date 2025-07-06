#ifndef __TASK_COLLECT_TELEMETRY_H__
#define __TASK_COLLECT_TELEMETRY_H__

#include "CustomTask.h"

class TaskCollectTelemetry : public CustomTask
{
public:
  TaskCollectTelemetry(Telemetry& telemetry, const esp_pthread_cfg_t& threadConfig)
    : CustomTask(telemetry, threadConfig) {}

  TaskCollectTelemetry(const TaskCollectTelemetry&) = delete;
  TaskCollectTelemetry(TaskCollectTelemetry&&) = delete;

  ~TaskCollectTelemetry() override = default;

  TaskCollectTelemetry& operator=(const TaskCollectTelemetry&) = delete;
  TaskCollectTelemetry& operator=(TaskCollectTelemetry&&) = delete;

protected:
  void threadLoop() override;

#endif // __TASK_COLLECT_TELEMETRY_H__