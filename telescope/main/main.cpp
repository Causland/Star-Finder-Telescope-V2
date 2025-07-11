#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <memory>

#include "CustomTask.h"
#include "TaskCollectTelemetry.h"
//#include "TaskControlCamera.h"
//#include "TaskFindPosition.h"
//#include "TaskMoveBaseServos.h"
//#include "TaskPlanTrajectory.h"
#include "TaskReceiveCommand.h"
#include "Tasks.h"
#include "Telemetry.h"

//TaskControlCamera controlCam{telemetry, Tasks::controlCameraCfg};
//TaskFindPosition findPos{telemetry, Tasks::findPositionCfg};
//TaskMoveBaseServos moveBaseServos{telemetry, Tasks::moveBaseServosCfg};
//TaskPlanTrajectory planTrajectory{telemetry, Tasks::planTrajectoryCfg};

extern "C" void app_main()
{
  auto collectTelem{std::make_shared<TaskCollectTelemetry>(Tasks::collectTelemCfg)};
  auto receiveCommand{std::make_shared<TaskReceiveCommand>(collectTelem->getTelemetry(),
                                                           Tasks::receiveCommandCfg)};

  // Create each task and populate shared Tasks object, then provide to each
  Tasks::setTask(collectTelem, Tasks::TASK_COLLECT_TELEMETRY);
//  Tasks::setTask(&controlCam, Tasks::TASK_CONTROL_CAMERA);
//  Tasks::setTask(&findPos, Tasks::TASK_FIND_POSITION);
//  Tasks::setTask(&moveBaseServos, Tasks::TASK_MOVE_BASE_SERVOS);
//  Tasks::setTask(&planTrajectory, Tasks::TASK_PLAN_TRAJECTORY);
  Tasks::setTask(receiveCommand, Tasks::TASK_RECEIVE_COMMAND);

  // Start all tasks
  Tasks::startTasks();
}