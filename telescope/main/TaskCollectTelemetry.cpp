#include <esp_log.h>

#include <Command.h>
#include "TaskCollectTelemetry.h"
#include "TelemRateCmd.h"

void TaskCollectTelemetry::threadLoop()
{
  while (!exitFlag)
  {
    // Wait for a telemetry timeout, command, or exit signal
    std::unique_lock<std::mutex> lk(cmdMutex);
    const bool cond = cv.wait_for(lk, telemRate,
                                  [this](){ return !cmdQueue.empty() || exitFlag; });
    if (!cond)
    {
      lk.unlock();

      // Telemetry timeout occured. Collect telemetry data and send
      const auto bytes{telemetry.serializeTelemetry()};
      if (bytes < 0)
      {
        ESP_LOGE(cfg.thread_name, "Failed to serialize telemetry data!");
        continue;
      }

      if (!telemSender.send(telemetry.getBuffer().data(), bytes))
      {
        ESP_LOGE(cfg.thread_name, "Failed to send telemetry data!");
        continue;
      }
    }
    else
    {
      // The condition was met, either a command was received or we are exiting
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

      // Deserialize command
      if (cmd->id == Command::CMD_TELEM_RATE)
      {
        auto telemRateCmd = static_pointer_cast<TelemRateCmd>(cmd);
        // Update the telemetry rate
        if (telemRateCmd->rate < MIN_TELEM_RATE_MS)
        {
          ESP_LOGW(cfg.thread_name, "Telemetry rate command is invalid: %d ms. "
                                    "Setting to minimum: %d ms",
                                    telemRateCmd->rate, MIN_TELEM_RATE_MS);
          telemRateCmd->rate = MIN_TELEM_RATE_MS;
        }

        telemRate = std::chrono::milliseconds(telemRateCmd->rate);
        ESP_LOGI(cfg.thread_name, "Telemetry rate updated to: %d ms", telemRateCmd->rate);
      }
      else
      {
        ESP_LOGW(cfg.thread_name, "Received unsupported command type: %d", static_cast<int>(cmd->id));
      }
    }
  }
}