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
    TASK_REMOTE_LOG,
    TASK_COLLECT_TELEMETRY,
    TASK_RECEIVE_COMMAND,
    TASK_MOVE_BASE_SERVOS,
    TASK_PLAN_TRAJECTORY,
    TASK_CONTROL_CAMERA,
    TASK_FIND_POSITION,
    TASK_FOCUS,
    TASK_OTA_UPDATE,
    NUM_TASKS,
  };

  static constexpr esp_pthread_cfg_t remoteLogCfg
                                      {4096, 2, false, "RemoteLog", 4, 0};
  static constexpr esp_pthread_cfg_t collectTelemCfg
                                      {4096, 2, false, "CollectTelem", 4, 0};
  static constexpr esp_pthread_cfg_t receiveCommandCfg
                                      {4096, 2, false, "ReceiveCommand", 7, 0};
  static constexpr esp_pthread_cfg_t moveBaseServosCfg
                                      {8192, 2, false, "MoveBaseServos", 6, 0};
  static constexpr esp_pthread_cfg_t planTrajectoryCfg
                                      {4096, 2, false, "PlanTrajectory", 5, 0};
  static constexpr esp_pthread_cfg_t controlCameraCfg
                                      {4096, 2, false, "ControlCamera", 5, 0};
  static constexpr esp_pthread_cfg_t findPositionCfg
                                      {2048, 2, false, "FindGPSPos", 3, 0};
  static constexpr esp_pthread_cfg_t focusCfg
                                      {4096, 2, false, "Focus", 6, 0};
  static constexpr esp_pthread_cfg_t otaUpdateCfg
                                      {8192, 2, false, "OTAUpdate", 7, 0};

  /// Set the task with the given ID to the provided task pointer. The pointer will be
  /// managed by the Tasks class and is moved into the tasks array.
  ///
  /// @param[in] task the task pointer
  /// @param[in] id the ID of the task to set.
  ///
  /// @return true if the task was set successfully, false if the ID is invalid.
  static bool setTask(std::shared_ptr<CustomTask> task, const TaskID& id);

  /// Get a pointer to the task with the given ID.
  static std::shared_ptr<CustomTask> getTask(const TaskID& id);

  /// Start all tasks in the system.
  static void startTasks();

  /// Stop all tasks in the system. This will wait for all tasks to finish before returning.
  static void stopTasks();

private:
  static std::array<std::shared_ptr<CustomTask>, NUM_TASKS> tasks; ///< Container for all tasks in the system.

  static constexpr const char* TAG{"Tasks"}; ///< The tag for logging.
};

#endif