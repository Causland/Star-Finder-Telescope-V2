#include <Arduino_FreeRTOS.h>
#include <lib/FreeRTOS-Kernel-v10.5.1\message_buffer.h>

#include "Tasks.h"
#include "Utils.h"

BaseType_t createTasks(void* taskParams[NUM_TASKS],
                       TaskHandle_t taskHandles[NUM_TASKS])
{
  DEBUG_ENTER("createTasks()");

  BaseType_t result = xTaskCreate(taskCollectTelemetry,
                                        TaskCollectTelemetryInfo::NAME,
                                        TaskCollectTelemetryInfo::STACK_DEPTH,
                                        taskParams[TASK_COLLECT_TELEMETRY],
                                        TaskCollectTelemetryInfo::PRIORITY,
                                        &taskHandles[TASK_COLLECT_TELEMETRY]);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Collect Telemetry task!");
  }

  result = xTaskCreate(taskReceiveCommand,
                       TaskReceiveCommandInfo::NAME,
                       TaskReceiveCommandInfo::STACK_DEPTH,
                       taskParams[TASK_RECEIVE_COMMAND],
                       TaskReceiveCommandInfo::PRIORITY,
                       &taskHandles[TASK_RECEIVE_COMMAND]);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Receive Command task!");
  }

  result = xTaskCreate(taskMoveServo,
                       TaskMoveServoInfo::NAME,
                       TaskMoveServoInfo::STACK_DEPTH,
                       taskParams[TASK_MOVE_SERVO],
                       TaskMoveServoInfo::PRIORITY,
                       &taskHandles[TASK_MOVE_SERVO]);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Move Servo task!");
  }

  DEBUG_EXIT("createTasks()");
  return result;
}