#include "Tasks.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>

std::array<CustomTask*, Tasks::NUM_TASKS> Tasks::tasks{};

bool Tasks::setTask(CustomTask* const task, const TaskID& id)
{
    if (id >= NUM_TASKS)
    {
        return false;
    }

    tasks[id] = task;
    return true;
}

CustomTask* Tasks::getTask(const TaskID& id)
{
    if (id >= NUM_TASKS)
    {
        return nullptr;
    }

    return tasks[id];
}

void Tasks::startTasks()
{
    for (auto* task : tasks)
    {
        if (!task)
        {
            ESP_LOGE(pcTaskGetName(NULL), "Task is a nullptr during start");
            continue;
        }
        task->start();
    }
}

void Tasks::stopTasks()
{
    for (auto* task : tasks)
    {
        if (!task)
        {
            ESP_LOGE(pcTaskGetName(NULL), "Task is a nullptr during stop");
            continue;
        }
        task->stop();
    }
}