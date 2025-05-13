#include <freertos/FreeRTOS.h>
#include <freertos/message_buffer.h>

#include "Tasks.h"
#include "Utils.h"

bool createTasks(void* taskParams[NUM_TASKS],
                 TaskHandle_t taskHandles[NUM_TASKS],
                 StackType_t taskStack[NUM_TASKS][TASK_STACK_DEPTH],
                 StaticTask_t taskTCB[NUM_TASKS])
{
  DEBUG_ENTER("createTasks()");

  taskHandles[TASK_COLLECT_TELEMETRY] = xTaskCreateStatic(
                                          taskCollectTelemetry,
                                          TaskCollectTelemetryInfo::NAME,
                                          TASK_STACK_DEPTH,
                                          taskParams[TASK_COLLECT_TELEMETRY],
                                          TaskCollectTelemetryInfo::PRIORITY,
                                          taskStack[TASK_COLLECT_TELEMETRY],
                                          &taskTCB[TASK_COLLECT_TELEMETRY]);
  if (taskHandles[TASK_COLLECT_TELEMETRY] == nullptr)
  {
    DEBUG_PRINTLN("Failed to create Collect Telemetry task!");
    return false;
  }

  taskHandles[TASK_RECEIVE_COMMAND] = xTaskCreateStatic(
                                          taskReceiveCommand,
                                          TaskReceiveCommandInfo::NAME,
                                          TASK_STACK_DEPTH,
                                          taskParams[TASK_RECEIVE_COMMAND],
                                          TaskReceiveCommandInfo::PRIORITY,
                                          taskStack[TASK_RECEIVE_COMMAND],
                                          &taskTCB[TASK_RECEIVE_COMMAND]);
  if (taskHandles[TASK_RECEIVE_COMMAND] == nullptr)
  {
    DEBUG_PRINTLN("Failed to create Receive Command task!");
    return false;
  }

  taskHandles[TASK_MOVE_BASE_SERVOS] = xTaskCreateStatic(
                                          taskMoveBaseServos,
                                          TaskMoveBaseServosInfo::NAME,
                                          TASK_STACK_DEPTH,
                                          taskParams[TASK_MOVE_BASE_SERVOS],
                                          TaskMoveBaseServosInfo::PRIORITY,
                                          taskStack[TASK_MOVE_BASE_SERVOS],
                                          &taskTCB[TASK_MOVE_BASE_SERVOS]);
  if (taskHandles[TASK_MOVE_BASE_SERVOS] == nullptr)
  {
    DEBUG_PRINTLN("Failed to create Move Base Servos task!");
    return false;
  }

  taskHandles[TASK_PLAN_TRAJECTORY] = xTaskCreateStatic(
                                          taskPlanTrajectory,
                                          TaskPlanTrajectoryInfo::NAME,
                                          TASK_STACK_DEPTH,
                                          taskParams[TASK_PLAN_TRAJECTORY],
                                          TaskPlanTrajectoryInfo::PRIORITY,
                                          taskStack[TASK_PLAN_TRAJECTORY],
                                          &taskTCB[TASK_PLAN_TRAJECTORY]);
  if (taskHandles[TASK_PLAN_TRAJECTORY] == nullptr)
  {
    DEBUG_PRINTLN("Failed to create Plan Trajectory task!");
    return false;
  }

  taskHandles[TASK_CONTROL_CAMERA] = xTaskCreateStatic(
                                          taskControlCamera,
                                          TaskControlCameraInfo::NAME,
                                          TASK_STACK_DEPTH,
                                          taskParams[TASK_CONTROL_CAMERA],
                                          TaskControlCameraInfo::PRIORITY,
                                          taskStack[TASK_CONTROL_CAMERA],
                                          &taskTCB[TASK_CONTROL_CAMERA]);
  if (taskHandles[TASK_CONTROL_CAMERA] == nullptr)
  {
    DEBUG_PRINTLN("Failed to create Control Camera task!");
    return false;
  }

  DEBUG_EXIT("createTasks()");
  return true;
}