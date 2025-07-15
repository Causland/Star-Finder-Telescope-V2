#include <esp_log.h>

#include "MoveBaseServosCmd.h"
#include "TaskPlanTrajectory.h"
#include "Tasks.h"

bool TaskPlanTrajectory::trajRunning{};
uint8_t TaskPlanTrajectory::numEntries{};
uint8_t TaskPlanTrajectory::currEntry{};
std::chrono::milliseconds TaskPlanTrajectory::timeToNextEntry{};

TaskPlanTrajectory::TaskPlanTrajectory(Telemetry& telemetry,
                                       const esp_pthread_cfg_t& threadConfig) :
                                        CustomTask(threadConfig)
{
  telemetry.registerTelemFieldTrajRunningCB(getTrajRunning);
  telemetry.registerTelemFieldTrajNumEntriesCB(getNumEntries);
  telemetry.registerTelemFieldTrajCurrEntryCB(getCurrEntry);
  telemetry.registerTelemFieldTimeToNextEntryCB(getTimeToNextEntry);
}

void TaskPlanTrajectory::threadLoop()
{
  while (!exitFlag)
  {
    // Wait for a command or exit signal
    std::unique_lock<std::mutex> lk(cmdMutex);
    cv.wait(lk, [this](){ return !cmdQueue.empty() || exitFlag; });
    
    if (exitFlag) break;

    if (cmdQueue.empty()) continue;

    // Get the command from the queue
    auto cmd = std::move(cmdQueue.front());
    cmdQueue.pop();
    lk.unlock();

    if (!cmd)
    {
      ESP_LOGE(cfg.thread_name, "Received null command!");
      continue;
    }

    // Process command
    if (cmd->id == Command::CMD_PLAN_TRAJECTORY)
    {
      auto trajCmd = std::static_pointer_cast<PlanTrajectoryCmd>(cmd);
      
      // Check if the sequence number is next expected
      if (trajCmd->header.seqNum != lastSeqNumReceived + 1)
      {
        ESP_LOGE(cfg.thread_name, "Received trajectory part out of order! Expected "
                                  "%d, got %d", lastSeqNumReceived + 1, trajCmd->header.seqNum);
        resetTrajectory();
        continue;
      }
      lastSeqNumReceived = trajCmd->header.seqNum;

      // Setup trajectory info if first part
      if (trajCmd->header.seqNum == 1)
      {
        if (trajCmd->header.totalParts > MAX_TRAJECTORY_PARTS)
        {
          ESP_LOGE(cfg.thread_name, "Trajectory has too many parts! %d > %d",
                                    trajCmd->header.totalParts, MAX_TRAJECTORY_PARTS);
          resetTrajectory();
          continue;
        }

        numParts = trajCmd->header.totalParts;
      }

      if (trajCmd->header.seqNum > numParts)
      {
        ESP_LOGE(cfg.thread_name, "Received trajectory part with sequence number "
                                  "%d greater than total parts %d",
                                  trajCmd->header.seqNum, numParts);
        resetTrajectory();
        continue;
      }

      if (trajCmd->header.totalParts != numParts)
      {
        ESP_LOGE(cfg.thread_name, "Received trajectory part with total parts %d "
                                  "does not match expected %d",
                                  trajCmd->header.totalParts, numParts);
        resetTrajectory();
        continue;
      }

      if (trajCmd->header.numEntries > PlanTrajectoryCmd::MAX_ENTRIES_PER_TRAJECTORY_PART)
      {
        ESP_LOGE(cfg.thread_name, "Received trajectory part with too many entries! %d > %d",
                                  trajCmd->header.numEntries, 
                                  PlanTrajectoryCmd::MAX_ENTRIES_PER_TRAJECTORY_PART);
        resetTrajectory();
        continue;
      }

      const auto begin{std::begin(trajCmd->entries)};
      std::copy(begin, begin + trajCmd->header.numEntries, 
                  std::begin(trajectory) + numEntries);
      numEntries += trajCmd->header.numEntries;

      if (trajCmd->header.seqNum == numParts)
      {
        // Last part received. Start processing
        ESP_LOGI(cfg.thread_name, "Received complete trajectory with %d entries", numEntries);

        const auto startTime{std::chrono::steady_clock::now()};
        auto moveServosTask{Tasks::getTask(Tasks::TASK_MOVE_BASE_SERVOS)};
        if (!moveServosTask)
        {
          ESP_LOGE(cfg.thread_name, "Unable to process trajectory due to nullptr"
                                    " move servos task");
          resetTrajectory();
          continue;
        } 

        processTrajectory(startTime, moveServosTask);
        resetTrajectory(); // We are done with this trajectory
      }
    }
    else
    {
      ESP_LOGW(cfg.thread_name, "Received unsupported command type: %d", static_cast<int>(cmd->id));
    }
  }
}

void TaskPlanTrajectory::resetTrajectory()
{
  lastSeqNumReceived = 0;
  numEntries = 0;
  currEntry = 0;
  numParts = 0;
  timeToNextEntry = std::chrono::milliseconds(0);
  trajRunning = false;
}

void TaskPlanTrajectory::processTrajectory(const std::chrono::steady_clock::time_point& startTime,
                                           std::shared_ptr<CustomTask>& moveServosTask)
{
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::steady_clock;

  trajRunning = true;

  for (size_t i=0; i<numEntries; ++i)
  {
    const auto& entry{trajectory[i]};
    currEntry = i + 1;

    const auto wakeUpTime{startTime + duration<float>(entry.t)};
    timeToNextEntry = duration_cast<milliseconds>(wakeUpTime - steady_clock::now());
    std::this_thread::sleep_until(wakeUpTime);

    ESP_LOGI(cfg.thread_name, "Processing trajectory entry: %f, %f, %f",
             entry.t, entry.az, entry.el);

    auto cmd{std::make_shared<MoveBaseServosCmd>(entry.az, entry.el)};
    moveServosTask->pushCmd(cmd);
  }

  ESP_LOGD(cfg.thread_name, "Trajectory complete");
}