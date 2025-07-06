#ifndef __TASKS_H__
#define __TASKS_H__

#include <esp_heap_caps.h>
#include <esp_log.h>
#include <esp_pthread.h>

#include "CustomTask.h"

class Tasks
{
public:
  /// UID for each task
  enum TaskID
  {
    TASK_COLLECT_TELEMETRY,
    TASK_RECEIVE_COMMAND,
    TASK_MOVE_BASE_SERVOS,
    TASK_PLAN_TRAJECTORY,
    TASK_CONTROL_CAMERA,
    TASK_FIND_POSITION,
    NUM_TASKS,
  };

  static constexpr esp_pthread_cfg_t collectTelemCfg
                                      {4096, 2, false, "CollectTelem", 1, MALLOC_CAP_DEFAULT};
  static constexpr esp_pthread_cfg_t receiveCommandCfg
                                      {4096, 2, false, "ReceiveCommand", 1, MALLOC_CAP_DEFAULT};
  static constexpr esp_pthread_cfg_t moveBaseServosCfg
                                      {8192, 2, false, "MoveBaseServos", 1, MALLOC_CAP_DEFAULT};
  static constexpr esp_pthread_cfg_t planTrajectoryCfg
                                      {4096, 2, false, "PlanTrajectory", 1, MALLOC_CAP_DEFAULT};
  static constexpr esp_pthread_cfg_t controlCameraCfg
                                      {4096, 2, false, "ControlCamera", 1, MALLOC_CAP_DEFAULT};
  static constexpr esp_pthread_cfg_t findPositionCfg
                                      {4096, 2, false, "FindGPSPos", 1, MALLOC_CAP_DEFAULT};

  static bool setTask(CustomTask* const task, const TaskID& id);
  static CustomTask* getTask(const TaskID& id);
  static void startTasks();
  static void stopTasks();

private:
  static std::array<CustomTask*, NUM_TASKS> tasks;
};

#endif