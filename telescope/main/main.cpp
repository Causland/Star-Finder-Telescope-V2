#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <memory>

#include "CustomTask.h"

#include "TaskMoveBaseServos.h" // Moved first to resolve issue with arduino and esp-idf IPADDR_NONE definition
#include "TaskCollectTelemetry.h"
#include "TaskControlCamera.h"
#include "TaskFindPosition.h"
#include "TaskPlanTrajectory.h"
#include "TaskReceiveCommand.h"
#include "TaskFocus.h"
#include "Tasks.h"
#include "Telemetry.h"

extern "C" void app_main()
{
  auto collectTelem{std::make_shared<TaskCollectTelemetry>(Tasks::collectTelemCfg)};
  auto receiveCommand{std::make_shared<TaskReceiveCommand>(collectTelem->getTelemetry(),
                                                           Tasks::receiveCommandCfg)};
  auto planTrajectory{std::make_shared<TaskPlanTrajectory>(collectTelem->getTelemetry(),
                                                           Tasks::planTrajectoryCfg)};
  auto moveBaseServos{std::make_shared<TaskMoveBaseServos>(collectTelem->getTelemetry(),
                                                           Tasks::moveBaseServosCfg)};
  auto findPos{std::make_shared<TaskFindPosition>(collectTelem->getTelemetry(),
                                                  Tasks::findPositionCfg)};
  auto controlCam{std::make_shared<TaskControlCamera>(collectTelem->getTelemetry(),
                                                      Tasks::controlCameraCfg)};
  auto focus{std::make_shared<TaskFocus>(collectTelem->getTelemetry(),
                                         Tasks::focusCfg)};

  // Create each task and populate shared Tasks object, then provide to each
  Tasks::setTask(collectTelem, Tasks::TASK_COLLECT_TELEMETRY);
  Tasks::setTask(controlCam, Tasks::TASK_CONTROL_CAMERA);
  Tasks::setTask(findPos, Tasks::TASK_FIND_POSITION);
  Tasks::setTask(moveBaseServos, Tasks::TASK_MOVE_BASE_SERVOS);
  Tasks::setTask(planTrajectory, Tasks::TASK_PLAN_TRAJECTORY);
  Tasks::setTask(receiveCommand, Tasks::TASK_RECEIVE_COMMAND);
  Tasks::setTask(focus, Tasks::TASK_FOCUS);

  // Start all tasks
  Tasks::startTasks();
}