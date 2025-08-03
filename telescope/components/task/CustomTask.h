#ifndef __CUSTOM_TASK_H__
#define __CUSTOM_TASK_H__

#include <array>
#include <condition_variable>
#include <esp_pthread.h>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "Command.h"

/// A custom task class that provides a thread and basic command handling.
class CustomTask
{
public:
  /// Create a CustomTask with the provided information.
  ///
  /// @param[in] name the name of the task.
  /// @param[in] coreID the core ID to run the task on.
  /// @param[in] stackSize the stack size for the task.
  /// @param[in] priority the priority of the task.
  CustomTask(const char* name, const int& coreID, 
             const size_t& stackSize, const size_t& priority);
  
  /// Create a CustomTask with the provided information. Provides a
  /// default thread configuration if not specified.
  ///
  /// @param[in] threadConfig the thread configuration to use for the task.
  ///                         If not specified, the default configuration is used.
  explicit CustomTask(const esp_pthread_cfg_t& threadConfig = esp_pthread_get_default_config());

  CustomTask(const CustomTask&) = delete;
  CustomTask(CustomTask&&) = delete;

  virtual ~CustomTask();

  CustomTask& operator=(const CustomTask&) = delete;
  CustomTask& operator=(CustomTask&&) = delete;

  /// @return the name of the task.
  const char* getName() const { return cfg.thread_name; }

  /// Start the task thread and begin the thread loop.
  void start();

  /// Stop the task thread and wait for it to finish.
  virtual void stop();

  /// Push a command to the task's command queue.
  ///
  /// @param[in] cmd pointer to the command to push to the queue.
  void pushCmd(std::shared_ptr<Command>&& cmd);

protected:
  /// The thread loop that runs the task logic. This should be
  /// implemented by derived classes to define the task's behavior.
  virtual void threadLoop() = 0;

  std::condition_variable cv; ///< Condition variable for signaling the task thread.
  std::queue<std::shared_ptr<Command>> cmdQueue; ///< Queue for commands to be processed by the task.
  std::mutex cmdMutex; ///< Mutex for protecting access to the command queue.

  bool exitFlag{false}; ///< Flag to indicate if the task should exit.

  esp_pthread_cfg_t cfg; ///< The configuration for the task thread, including stack size and priority.

private:
  std::thread thread; ///< The underlying thread for the task. This should not need to be
                      ///< accessed directly by derived classes.
};

#endif