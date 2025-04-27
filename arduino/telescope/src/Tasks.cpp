#include <Arduino_FreeRTOS.h>

#include "Tasks.h"
#include "Utils.h"

BaseType_t createTasks(void* taskParams[NUM_TASKS],
                       TaskHandle_t* taskHandles[NUM_TASKS])
{
  DEBUG_ENTER("createTasks()");
  BaseType_t result = xTaskCreate(taskCollectTelemetry,
                                  TaskCollectTelemetryInfo::NAME,
                                  TaskCollectTelemetryInfo::STACK_DEPTH,
                                  taskParams[TASK_COLLECT_TELEMETRY],
                                  TaskCollectTelemetryInfo::PRIORITY,
                                  taskHandles[TASK_COLLECT_TELEMETRY]);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Collect Telemetry task!");
    return result;
  }

  result = xTaskCreate(taskReceiveCommand,
                       TaskReceiveCommandInfo::NAME,
                       TaskReceiveCommandInfo::STACK_DEPTH,
                       taskParams[TASK_RECEIVE_COMMAND],
                       TaskReceiveCommandInfo::PRIORITY,
                       taskHandles[TASK_RECEIVE_COMMAND]);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Receive Command task!");
    return result;
  }

  result = xTaskCreate(taskMoveServo,
                       TaskMoveServoInfo::NAME,
                       TaskMoveServoInfo::STACK_DEPTH,
                       taskParams[TASK_MOVE_SERVO],
                       TaskMoveServoInfo::PRIORITY,
                       taskHandles[TASK_MOVE_SERVO]);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Move Servo task!");
    return result;
  }

  DEBUG_EXIT("createTasks()");
  return pdPASS;
}