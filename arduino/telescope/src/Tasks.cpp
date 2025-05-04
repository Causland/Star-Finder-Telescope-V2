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

  result = xTaskCreate(taskMoveBaseServos,
                       TaskMoveBaseServosInfo::NAME,
                       TaskMoveBaseServosInfo::STACK_DEPTH,
                       taskParams[TASK_MOVE_BASE_SERVOS],
                       TaskMoveBaseServosInfo::PRIORITY,
                       &taskHandles[TASK_MOVE_BASE_SERVOS]);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Move Servo task!");
  }

  result = xTaskCreate(taskPlanTrajectory,
                       TaskPlanTrajectoryInfo::NAME,
                       TaskPlanTrajectoryInfo::STACK_DEPTH,
                       taskParams[TASK_PLAN_TRAJECTORY],
                       TaskPlanTrajectoryInfo::PRIORITY,
                       &taskHandles[TASK_PLAN_TRAJECTORY]);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Plan Trajectory task!");
  }

  DEBUG_EXIT("createTasks()");
  return result;
}