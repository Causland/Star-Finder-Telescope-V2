#ifndef __TASKS_H__
#define __TASKS_H__

#include <esp_heap_caps.h>
#include <esp_pthread.h>

#include "CustomTask.h"

/// Completely static class that manages all tasks in the system.
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

  /// Set the task with the given ID to the provided task pointer. The pointer will be
  /// managed by the Tasks class and is moved into the tasks array.
  ///
  /// @param[in] task the task pointer
  /// @param[in] id the ID of the task to set.
  ///
  /// @return true if the task was set successfully, false if the ID is invalid.
  static bool setTask(std::unique_ptr<CustomTask>& task, const TaskID& id);

  /// Get a pointer to the task with the given ID. Note - this is accessing a managed pointer,
  /// and should not be stored.
  static CustomTask* getTask(const TaskID& id);

  /// Start all tasks in the system.
  static void startTasks();

  /// Stop all tasks in the system. This will wait for all tasks to finish before returning.
  static void stopTasks();

private:
  static std::array<std::unique_ptr<CustomTask>, NUM_TASKS> tasks; ///< Container for all tasks in the system.

  static constexpr const char* TAG{"Tasks"}; ///< The tag for logging.
};

#endif