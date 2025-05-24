#ifndef __CUSTOM_TASK_H__
#define __CUSTOM_TASK_H__

#include <array>
#include <condition_variable>
#include <esp_log.h>
#include <esp_pthread.h>
#include <mutex>
#include <queue>
#include <thread>

#include "Command.h"
#include "Telemetry.h"

class CustomTask
{
public:
  CustomTask(Telemetry& telemetry, const char* name, const int& coreID, 
             const size_t& stackSize, const size_t& priority);
  explicit CustomTask(Telemetry& telemetry,
    const esp_pthread_cfg_t& threadConfig = esp_pthread_get_default_config());

  CustomTask(const CustomTask&) = delete;
  CustomTask(CustomTask&&) = delete;

  virtual ~CustomTask();

  CustomTask& operator=(const CustomTask&) = delete;
  CustomTask& operator=(CustomTask&&) = delete;

  void start();
  void stop();

  void pushCmd(Command&& cmd);

protected:
  virtual void threadLoop() = 0;

  std::condition_variable cv;
  std::queue<Command> cmdQueue;
  std::mutex cmdMutex;

  bool exitFlag{false};

private:
  std::thread thread;
  esp_pthread_cfg_t cfg;
};

#endif