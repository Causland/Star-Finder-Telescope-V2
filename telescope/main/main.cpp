#include <freertos/FreeRTOS.h>

#include "CustomTask.h"
#include "Telemetry.h"

// Define globals which must exist after the main task exits
Telemetry telemetry;

/*TaskCollectTelemetry collectTelem{telemetry, Tasks::collectTelemCfg};
TaskControlCamera controlCam{telemetry, Tasks::controlCameraCfg};
TaskFindPosition findPos{telemetry, Tasks::findPositionCfg};
TaskMoveBaseServos moveBaseServos{telemetry, Tasks::moveBaseServosCfg};
TaskPlanTrajectory planTrajectory{telemetry, Tasks::planTrajectoryCfg};
TaskReceiveCommand receiveCmd{telemetry, Tasks::receiveCommandCfg};*/

extern "C" void app_main()
{
  // Create each task and populate shared Tasks object, then provide to each
/*  Tasks::setTask(&collectTelem, Tasks::TASK_COLLECT_TELEMETRY);
  Tasks::setTask(&controlCam, Tasks::TASK_CONTROL_CAMERA);
  Tasks::setTask(&findPos, Tasks::TASK_FIND_POSITION);
  Tasks::setTask(&moveBaseServos, Tasks::TASK_MOVE_BASE_SERVOS);
  Tasks::setTask(&planTrajectory, Tasks::TASK_PLAN_TRAJECTORY);
  Tasks::setTask(&receiveCmd, Tasks::TASK_RECEIVE_COMMAND);

  // Start all tasks
  Tasks::startTasks();*/
}