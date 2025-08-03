#ifndef __TASK_CONTROL_CAMERA_H__
#define __TASK_CONTROL_CAMERA_H__

#include <array>
#include <chrono>

#include "Camera.h"
#include "CustomTask.h"
#include "Telemetry.h"

class TaskControlCamera : public CustomTask
{
public:
  explicit TaskControlCamera(const esp_pthread_cfg_t& threadConfig);

  TaskControlCamera(const TaskControlCamera&) = delete;
  TaskControlCamera(TaskControlCamera&&) = delete;

  ~TaskControlCamera() override = default;

  TaskControlCamera& operator=(const TaskControlCamera&) = delete;
  TaskControlCamera& operator=(TaskControlCamera&&) = delete;

protected:
  void threadLoop() override;

private:
    Camera cam;
};

#endif // __TASK_CONTROL_CAMERA_H__