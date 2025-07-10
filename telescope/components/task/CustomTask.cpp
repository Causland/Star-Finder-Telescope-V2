#include <esp_heap_caps.h>
#include <esp_log.h>

#include "CustomTask.h"

CustomTask::CustomTask(const char* name, const int& coreID, 
                       const size_t& stackSize, const size_t& priority) :
    cfg({stackSize, priority, false, name, coreID, MALLOC_CAP_DEFAULT}) 
{
}

CustomTask::CustomTask(const esp_pthread_cfg_t& threadConfig) :
    cfg(threadConfig) 
{
}

CustomTask::~CustomTask()
{
    stop();
}

void CustomTask::start()
{
    ESP_LOGD(cfg.thread_name, "Starting task on core %d with stack size %zu and priority %zu",
             cfg.pin_to_core, cfg.stack_size, cfg.prio);

    if (thread.joinable())
    {
        stop();
    }

    exitFlag = false;

    esp_pthread_set_cfg(&cfg);
    thread = std::thread(&CustomTask::threadLoop, this);
}

void CustomTask::stop()
{
    ESP_LOGD(cfg.thread_name, "Stopping task");

    {
        std::scoped_lock lk{cmdMutex};
        exitFlag = true;
        cv.notify_one();
    }

    if (thread.joinable())
    {
        thread.join();
    }
}

void CustomTask::pushCmd(std::shared_ptr<Command>&& cmd)
{
    std::scoped_lock lk{cmdMutex};
    cmdQueue.push(std::move(cmd));
    cv.notify_one();
}