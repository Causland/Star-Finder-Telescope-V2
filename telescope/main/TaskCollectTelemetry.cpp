#include "TaskCollectTelemetry.h"
#include "TelemetryRateCmd.h"

void threadLoop()
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
      Command cmd = std::move(cmdQueue.front());
      cmdQueue.pop();
      lk.unlock();

      // Deserialize command
      if (cmd.id == CommandID::CMD_TELEM_RATE)
      {
        // Should be deserialized already from receive command task
        TelemRateCmd& telemRateCmd = static_cast<TelemRateCmd&>(cmd);
        {
          // Update the telemetry rate
          if (telemRateCmd.rate < MIN_TELEM_RATE_MS)
          {
            ESP_LOGW(cfg.thread_name, "Telemetry rate command is invalid: %d ms. "
                                      "Setting to minimum: %d ms",
                                      telemRateCmd.rate, MIN_TELEM_RATE_MS);
            telemRateCmd.rate = MIN_TELEM_RATE_MS;
          }

          telemRate = std::chrono::milliseconds(telemRateCmd.rate);
          ESP_LOGI(cfg.thread_name, "Telemetry rate updated to: %d ms", telemRateCmd.rate);
        }
        else
        {
          ESP_LOGE(cfg.thread_name, "Failed to deserialize telemetry rate command!");
        }
      }
      else
      {
        ESP_LOGW(cfg.thread_name, "Received unsupported command type: %d", static_cast<int>(cmd.type));
      }
    }
  }
}