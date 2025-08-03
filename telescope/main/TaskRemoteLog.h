#ifndef __TASK_REMOTE_LOG_H__
#define __TASK_REMOTE_LOG_H__

#include "CustomTask.h"
#include "UDPSender.h"
#include "NetworkConstants.h"

class TaskRemoteLog : public CustomTask
{
public:
  explicit TaskRemoteLog(const esp_pthread_cfg_t& threadConfig);

  TaskRemoteLog(const TaskRemoteLog&) = delete;
  TaskRemoteLog(TaskRemoteLog&&) = delete;

  ~TaskRemoteLog() override = default;

  TaskRemoteLog& operator=(const TaskRemoteLog&) = delete;
  TaskRemoteLog& operator=(TaskRemoteLog&&) = delete;

  void stop() override
  {
    exitFlag = true;
    logCondVar.notify_one();
  }

protected:
  void threadLoop() override;

private:
  static constexpr size_t MAX_LOG_SIZE{128}; ///< Maximum size of a log message

  static int logOutput(const char* fmt, va_list args);

  UDPSender logSender{"LogSender", WIFI_USER_ADDR, WIFI_LOG_PORT};

  static std::queue<std::string> logQueue;
  static std::mutex logMutex;
  static std::condition_variable logCondVar;
};

#endif // __TASK_REMOTE_LOG_H__