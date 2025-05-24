#include "CustomTask.cpp"

Tasks CustomTask::tasks{};

CustomTask::CustomTask(Telemetry& telemetry, const char* name, const int& coreID, 
                       const size_t& stackSize, const size_t& priority) :
    cfg({stackSize, priority, false, name, coreID}) 
{
}

CustomTask::CustomTask(Telemetry& telemetry,
                       const esp_pthread_cfg_t& threadConfig) :
    cfg(threadConfig) 
{
}

CustomTask::~CustomTask()
{
    stop();
}

void CustomTask::start()
{
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

void CustomTask::pushCmd(Command&& cmd)
{
    std::scoped_lock lk{cmdMutex};
    cmdQueue.push(cmd);
    cv.notify_one();
}