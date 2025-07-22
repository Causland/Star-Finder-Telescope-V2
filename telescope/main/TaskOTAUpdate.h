#ifndef __TASK_OTA_UPDATE_H__
#define __TASK_OTA_UPDATE_H__

#include <chrono>

#include "CustomTask.h"
#include "OTAUpdateCmd.h"
#include "OTAUpdater.h"
#include "Telemetry.h"

class TaskOTAUpdate : public CustomTask
{
public:
  TaskOTAUpdate(Telemetry& telemetry, const esp_pthread_cfg_t& threadConfig);

  TaskOTAUpdate(const TaskOTAUpdate&) = delete;
  TaskOTAUpdate(TaskOTAUpdate&&) = delete;

  ~TaskOTAUpdate() override = default;

  TaskOTAUpdate& operator=(const TaskOTAUpdate&) = delete;
  TaskOTAUpdate& operator=(TaskOTAUpdate&&) = delete;

protected:
  void threadLoop() override;

private:
  OTAUpdater ota;
};

#endif // __TASK_OTA_UPDATE_H__