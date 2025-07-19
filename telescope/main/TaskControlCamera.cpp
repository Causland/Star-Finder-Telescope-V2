#include <chrono>
#include <esp_log.h>
#include <thread>

#include "ControlCameraCmd.h"
#include "TaskControlCamera.h"

TaskControlCamera::TaskControlCamera(Telemetry& telemetry, 
                                     const esp_pthread_cfg_t& threadConfig)
                                      : CustomTask(threadConfig)
{

}

void TaskControlCamera::threadLoop()
{
  // Attempt to initialize the camera
  while (!cam.initialize())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

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

    if (cmd->id == Command::CMD_CONTROL_CAMERA)
    {
      auto camCmd = std::static_pointer_cast<ControlCameraCmd>(cmd);
      switch(camCmd->ctrlCmdID)
      {
        case ControlCameraCmd::CTRL_CAM_CONFIG:
        {
          ESP_LOGD(cfg.thread_name, "Received cam config command. ID=%u val=%u",
                                    camCmd->cfg.configID, camCmd->cfg.value);
          break;
        }
        case ControlCameraCmd::CTRL_CAM_PHOTO:
        {
          ESP_LOGD(cfg.thread_name, "Received take photo command");
          cam.takePhoto();
          break;
        }
        case ControlCameraCmd::CTRL_CAM_VIDEO:
        {
          ESP_LOGD(cfg.thread_name, "Received take video command. dur=%lus", camCmd->vid.duration);
          cam.takeVideo();
          break;
        }
        default:
        {
          ESP_LOGE(cfg.thread_name, "Unknown camera cmd ID %i", camCmd->ctrlCmdID);
          continue;
        }
      }
    }
    else
    {
      ESP_LOGW(cfg.thread_name, "Received unsupported command type: %d", static_cast<int>(cmd->id));
    }
  }
}