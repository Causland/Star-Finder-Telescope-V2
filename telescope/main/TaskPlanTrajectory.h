#ifndef __TASK_PLAN_TRAJECTORY_H__
#define __TASK_PLAN_TRAJECTORY_H__

#include <array>
#include <chrono>

#include "CustomTask.h"
#include "PlanTrajectoryCmd.h"
#include "Telemetry.h"

class TaskPlanTrajectory : public CustomTask
{
public:
  TaskPlanTrajectory(Telemetry& telemetry, const esp_pthread_cfg_t& threadConfig);

  TaskPlanTrajectory(const TaskPlanTrajectory&) = delete;
  TaskPlanTrajectory(TaskPlanTrajectory&&) = delete;

  ~TaskPlanTrajectory() override = default;

  TaskPlanTrajectory& operator=(const TaskPlanTrajectory&) = delete;
  TaskPlanTrajectory& operator=(TaskPlanTrajectory&&) = delete;

  static bool getTrajRunning() { return trajRunning; }
  static uint8_t getNumEntries() { return numEntries; }
  static uint8_t getCurrEntry() { return currEntry; }
  static int64_t getTimeToNextEntry() { return timeToNextEntry.count();}

protected:
  void threadLoop() override;

private:
  /// Reset the trajectory variables in the task
  void resetTrajectory();

  /// Process the trajectory by iterating through each entry and sending commands
  /// to the servos at the specified time
  ///
  /// @param[in] startTime The start time of the trajectory. Used to determine
  ///                      the relative timing of entries
  /// @param[in] moveServosTask Pointer to a valid move servo task to issue move command
  void processTrajectory(const std::chrono::steady_clock::time_point& startTime,
                         std::shared_ptr<CustomTask>& moveServosTask);

  static constexpr uint8_t MAX_TRAJECTORY_PARTS{4}; ///< Max number of parts in a trajectory

  std::array<PlanTrajectoryCmd::TrajectoryEntry_t,
             PlanTrajectoryCmd::MAX_ENTRIES_PER_TRAJECTORY_PART * MAX_TRAJECTORY_PARTS> trajectory{};
  uint8_t lastSeqNumReceived{}; ///< The last sequence number received in a part.
                                ///< Sequence numbers are 1 indexed. A 0 in this variable
                                ///< indicates that no parts have been received
  uint8_t numParts{}; ///< Total number of parts in the current trajectory
  
  static bool trajRunning; ///< Flag indicating if a trajectory is currently running
  static uint8_t numEntries; ///< Number of entries in the current trajectory
  static uint8_t currEntry; ///< Current entry being executed
  static std::chrono::milliseconds timeToNextEntry; ///< Time to the next entry in the trajectory
};

#endif // __TASK_PLAN_TRAJECTORY_H__