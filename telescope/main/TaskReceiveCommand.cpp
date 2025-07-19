#include <esp_log.h>

#include "TaskReceiveCommand.h"
#include "Tasks.h"

#include "ControlCameraCmd.h"
#include "MoveBaseServosCmd.h"
#include "PlanTrajectoryCmd.h"
#include "TelemRateCmd.h"

uint16_t TaskReceiveCommand::cmdsReceived{};

TaskReceiveCommand::TaskReceiveCommand(Telemetry& telemetry,
                                       const esp_pthread_cfg_t& threadConfig) :
                                        CustomTask(threadConfig)
{
  telemetry.registerTelemFieldCmdsReceivedCB(getCmdsReceived);
}

void TaskReceiveCommand::threadLoop()
{
  while (!exitFlag)
  {
    // Receive commands from the network interface
    const auto bytesReceived{cmdReceiver.receive(cmdBuffer.data(), cmdBuffer.size())};
    if (bytesReceived < 0)
    {
      if (!exitFlag)
      { 
        ESP_LOGE(cfg.thread_name, "Failed to receive command!");
        continue;
      }
    }

    const auto cmdTypeVal = cmdBuffer[Command::POS_CMD_ID];
    if (cmdTypeVal >= Command::NUM_COMMANDS)
    {
      ESP_LOGE(cfg.thread_name, "Received invalid command type: %d", cmdTypeVal);
      continue;
    }
    const auto cmdType = static_cast<Command::CommandID>(cmdTypeVal);

    std::shared_ptr<Command> cmd;
    std::shared_ptr<CustomTask> destTask;
    switch (cmdType)
    {
      case Command::CMD_TELEM_RATE:
      {
        cmd = std::make_shared<TelemRateCmd>();
        if (!cmd)
        {
          ESP_LOGE(cfg.thread_name, "Failed to create telemetry rate command!");
          continue;
        }
        ESP_LOGD(cfg.thread_name, "Received telemetry rate command!");
        destTask = Tasks::getTask(Tasks::TASK_COLLECT_TELEMETRY);
        break;
      }
      case Command::CMD_PLAN_TRAJECTORY:
      {
        cmd = std::make_shared<PlanTrajectoryCmd>();
        if (!cmd)
        {
          ESP_LOGE(cfg.thread_name, "Failed to create plan trajectory command!");
          continue;
        }
        ESP_LOGD(cfg.thread_name, "Received plan trajectory command!");
        destTask = Tasks::getTask(Tasks::TASK_PLAN_TRAJECTORY);
        break;
      }
      case Command::CMD_MOVE_BASE_SERVOS:
      {
        cmd = std::make_shared<MoveBaseServosCmd>();
        if (!cmd)
        {
          ESP_LOGE(cfg.thread_name, "Failed to create move servo command!");
          continue;
        }
        ESP_LOGD(cfg.thread_name, "Received move servo command!");
        destTask = Tasks::getTask(Tasks::TASK_MOVE_BASE_SERVOS);
        break;
      }
      case Command::CMD_CONTROL_CAMERA:
      {
        cmd = std::make_shared<ControlCameraCmd>();
        if (!cmd)
        {
          ESP_LOGE(cfg.thread_name, "Failed to create control camera command!");
          continue;
        }
        ESP_LOGD(cfg.thread_name, "Received control camera command!");
        destTask = Tasks::getTask(Tasks::TASK_CONTROL_CAMERA);
        break;
      }
      default:
      {
        ESP_LOGE(cfg.thread_name, "Received unknown command type: %d", cmdTypeVal);
        continue;
      }
    }

    ++cmdsReceived;
    if (!cmd->deserializeCommand(cmdBuffer.data(), bytesReceived))
    {
      ESP_LOGE(cfg.thread_name, "Failed to deserialize command %d!", cmdTypeVal);
      continue;
    }

    if (!destTask)
    {
      ESP_LOGE(cfg.thread_name, "No destination task found for command type: %d", cmdTypeVal);
      continue;
    }

    destTask->pushCmd(std::move(cmd));
    ESP_LOGD(cfg.thread_name, "Command of type %d sent to task %s",
            cmdTypeVal, destTask->getName());
  }
}