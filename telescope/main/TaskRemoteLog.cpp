#include <esp_log.h>

#include "TaskRemoteLog.h"
#include "WifiInitializer.h"

std::queue<std::string> TaskRemoteLog::logQueue{};
std::mutex TaskRemoteLog::logMutex{};
std::condition_variable TaskRemoteLog::logCondVar{};

TaskRemoteLog::TaskRemoteLog(const esp_pthread_cfg_t& threadConfig)
                : CustomTask(threadConfig)
{
  // Configure system logging to redirect to UDP once a client is connected
  esp_log_set_vprintf(logOutput);
}

void TaskRemoteLog::threadLoop()
{
  while (!exitFlag)
  {
    std::unique_lock lk{logMutex};
    logCondVar.wait(lk, [this](){ return !logQueue.empty() || exitFlag; });
    lk.unlock();

    if (exitFlag) break;

    std::string str;
    while (true)
    {
      {
        std::scoped_lock<std::mutex> slk{logMutex};
        if (logQueue.empty())
        {
          break; // Nothing left to send
        }
        str = std::move(logQueue.front());
        logQueue.pop();
      }

      if (WifiInitializer::isClientConnected())
      {
        logSender.send(reinterpret_cast<const uint8_t*>(str.data()),
                       str.size());
      }
      else
      {
        std::printf("%s", str.c_str());
      }
    }
  }
}

int TaskRemoteLog::logOutput(const char* fmt, va_list args)
{
  thread_local char buf[128]{};
  const int len{std::vsnprintf(buf, sizeof(buf), fmt, args)};

  {
    std::scoped_lock<std::mutex> lk{logMutex};
    logQueue.emplace(buf);
    logCondVar.notify_one();
  }

  return len;
}