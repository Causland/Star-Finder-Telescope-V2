#include <freertos/FreeRTOS.h>

#include "CustomTask.h"
//#include "TaskCollectTelemetry.h"
//#include "TaskControlCamera.h"
//#include "TaskFindPosition.h"
//#include "TaskMoveBaseServos.h"
//#include "TaskPlanTrajectory.h"
//#include "TaskReceiveCommand.h"
#include "Tasks.h"
#include "Telemetry.h"
#include "UDPReceiver.h"
#include "NetworkConstants.h"

// Define globals which must exist after the main task exits
Telemetry telemetry;

//TaskCollectTelemetry collectTelem{telemetry, Tasks::collectTelemCfg};
//TaskControlCamera controlCam{telemetry, Tasks::controlCameraCfg};
//TaskFindPosition findPos{telemetry, Tasks::findPositionCfg};
//TaskMoveBaseServos moveBaseServos{telemetry, Tasks::moveBaseServosCfg};
//TaskPlanTrajectory planTrajectory{telemetry, Tasks::planTrajectoryCfg};
//TaskReceiveCommand receiveCmd{telemetry, Tasks::receiveCommandCfg};



extern "C" void app_main()
{
  // Create each task and populate shared Tasks object, then provide to each
//  Tasks::setTask(&collectTelem, Tasks::TASK_COLLECT_TELEMETRY);
//  Tasks::setTask(&controlCam, Tasks::TASK_CONTROL_CAMERA);
//  Tasks::setTask(&findPos, Tasks::TASK_FIND_POSITION);
//  Tasks::setTask(&moveBaseServos, Tasks::TASK_MOVE_BASE_SERVOS);
//  Tasks::setTask(&planTrajectory, Tasks::TASK_PLAN_TRAJECTORY);
//  Tasks::setTask(&receiveCmd, Tasks::TASK_RECEIVE_COMMAND);

  // Start all tasks
  Tasks::startTasks();

  uint8_t data[100]{0};
  UDPReceiver udpReceiver{"receiver", WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT};
  while (true)
  {
    ssize_t receivedBytes = udpReceiver.receive(data, sizeof(data));
    if (receivedBytes > 0)
    {
      ESP_LOGI("UDPReceiver", "Received %zd bytes", receivedBytes);
      printf("Data: %s\n", data);
    }
    else if (receivedBytes < 0)
    {
      ESP_LOGE("UDPReceiver", "Error receiving data");
    }
  }
}