
#include <esp_log.h>

#include "TaskOTAUpdate.h"
#include "Tasks.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskOTAUpdate::TaskOTAUpdate(Telemetry& telemetry, const esp_pthread_cfg_t& threadConfig) :
                      CustomTask(threadConfig)
{
  telemetry.registerTelemFieldVersionCB(ota.getAppVersion);
}

void TaskOTAUpdate::threadLoop()
{
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
        if (cmd->id == Command::CMD_OTA_UPDATE)
        {
          auto otaCmd = std::static_pointer_cast<OTAUpdateCmd>(cmd);
          if (!ota.performUpdate(otaCmd->getFilename()))
          {
            ESP_LOGE(cfg.thread_name, "Failed to OTA update");
          }
        }
        else
        {
          ESP_LOGW(cfg.thread_name, "Received unsupported command type: %d", static_cast<int>(cmd->id));
        }
      }
    }
  }
}