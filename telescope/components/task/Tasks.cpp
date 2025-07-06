#include <esp_log.h>

#include "Tasks.h"

std::array<std::unique_ptr<CustomTask>, Tasks::NUM_TASKS> Tasks::tasks{};

bool Tasks::setTask(std::unique_ptr<CustomTask>& task, const TaskID& id)
{
  if (id >= NUM_TASKS)
  {
    return false;
  }

  tasks[id] = std::move(task);
  return true;
}

CustomTask* Tasks::getTask(const TaskID& id)
{
  if (id >= NUM_TASKS)
  {
    return nullptr;
  }

  return tasks[id].get();
}

void Tasks::startTasks()
{
  for (auto& task : tasks)
  {
    if (!task)
    {
      ESP_LOGE(TAG, "Task is a nullptr while starting");
      continue;
    }
    task->start();
  }
}

void Tasks::stopTasks()
{
  for (auto& task : tasks)
  {
    if (!task)
    {
      ESP_LOGE(TAG, "Task is a nullptr while stopping");
      continue;
    }
    task->stop();
  }
}