
#include <esp_log.h>

#include "TaskFocus.h"
#include "Tasks.h"

TaskFocus::TaskFocus(Telemetry& telemetry, const esp_pthread_cfg_t& threadConfig) :
                      CustomTask(threadConfig)
{
}

void TaskFocus::threadLoop()
{
  // Setup servo before starting forever loop
  focusServo.init();

  while (!exitFlag)
  {
    // Wait for a command or exit signal
    std::unique_lock<std::mutex> lk(cmdMutex);
    cv.wait(lk, [this](){ return !cmdQueue.empty() || exitFlag; });
    
    if (exitFlag) break;

    if (!cmdQueue.empty())
    {
      // Get the command from the queue
      auto cmd = std::move(cmdQueue.front());
      cmdQueue.pop();
      lk.unlock();

      if (!cmd)
      {
        ESP_LOGE(cfg.thread_name, "Received null command!");
      }
      else
      {
        if (cmd->id == Command::CMD_FOCUS)
        {
          auto focusCmd = std::static_pointer_cast<FocusCmd>(cmd);
          switch(focusCmd->focusCmdID)
          {
            case FocusCmd::FOCUS_MANUAL:
            {
              ESP_LOGI(cfg.thread_name, "Received focus manual command. val=%f",
                                        focusCmd->man.degrees);
              rotateFocus(focusCmd->man.degrees);
              break;
            }
            case FocusCmd::FOCUS_AUTO:
            {
              ESP_LOGI(cfg.thread_name, "Received focus auto command");
              break;
            }
            default:
            {
              ESP_LOGE(cfg.thread_name, "Unknown focus cmd ID %i", focusCmd->focusCmdID);
              continue;
            }
          }
        }
      }
    }
  }
}

void TaskFocus::rotateFocus(const float& degrees)
{
  // Move the focus servo by the angle specified at the slowest rate
  const int numUs{degrees >= 0.0 ? FOCUS_SERVO_STOP_US + FOCUS_SERVO_MIN_SPEED_OFFSET_US :
                                   FOCUS_SERVO_STOP_US - FOCUS_SERVO_MIN_SPEED_OFFSET_US};

  const auto sleepDur{std::chrono::duration<float>(degrees / FOCUS_SERVO_MIN_SPEED_DPS)};
  focusServo.writeMicroseconds(numUs);
  std::this_thread::sleep_for(sleepDur);
  focusServo.writeMicroseconds(FOCUS_SERVO_STOP_US);
}