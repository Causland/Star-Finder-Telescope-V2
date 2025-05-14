#include <freertos/FreeRTOS.h>
#include <freertos/message_buffer.h>

#include "Tasks.h"
#include "Utils.h"

BaseType_t createTasks(void* taskParams[NUM_TASKS],
                       TaskHandle_t taskHandles[NUM_TASKS])
{
  DEBUG_ENTER("createTasks()");

  BaseType_t result = xTaskCreatePinnedToCore(
                                  taskCollectTelemetry,
                                  TaskCollectTelemetryInfo::NAME,
                                  TaskCollectTelemetryInfo::STACK_DEPTH,
                                  taskParams[TASK_COLLECT_TELEMETRY],
                                  TaskCollectTelemetryInfo::PRIORITY,
                                  &taskHandles[TASK_COLLECT_TELEMETRY],
                                  1);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Collect Telemetry task!");
    return result;
  }

  vTaskDelay(100 * portTICK_PERIOD_MS);

  result = xTaskCreatePinnedToCore(taskReceiveCommand,
                                   TaskReceiveCommandInfo::NAME,
                                   TaskReceiveCommandInfo::STACK_DEPTH,
                                   taskParams[TASK_RECEIVE_COMMAND],
                                   TaskReceiveCommandInfo::PRIORITY,
                                   &taskHandles[TASK_RECEIVE_COMMAND],
                                   1);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Receive Command task!");
    return result;
  }

  vTaskDelay(100 * portTICK_PERIOD_MS);

  result = xTaskCreatePinnedToCore(taskMoveBaseServos,
                                   TaskMoveBaseServosInfo::NAME,
                                   TaskMoveBaseServosInfo::STACK_DEPTH,
                                   taskParams[TASK_MOVE_BASE_SERVOS],
                                   TaskMoveBaseServosInfo::PRIORITY,
                                   &taskHandles[TASK_MOVE_BASE_SERVOS],
                                   1);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Move Servo task!");
    return result;
  }

  vTaskDelay(100 * portTICK_PERIOD_MS);

  result = xTaskCreatePinnedToCore(taskPlanTrajectory,
                                   TaskPlanTrajectoryInfo::NAME,
                                   TaskPlanTrajectoryInfo::STACK_DEPTH,
                                   taskParams[TASK_PLAN_TRAJECTORY],
                                   TaskPlanTrajectoryInfo::PRIORITY,
                                   &taskHandles[TASK_PLAN_TRAJECTORY],
                                   1);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Plan Trajectory task!");
    return result;
  }

  vTaskDelay(100 * portTICK_PERIOD_MS);

  result = xTaskCreatePinnedToCore(taskControlCamera,
                                   TaskControlCameraInfo::NAME,
                                   TaskControlCameraInfo::STACK_DEPTH,
                                   taskParams[TASK_CONTROL_CAMERA],
                                   TaskControlCameraInfo::PRIORITY,
                                   &taskHandles[TASK_CONTROL_CAMERA],
                                   1);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create Control Camera task!");
    return result;
  }

  DEBUG_EXIT("createTasks()");
  return result;
}