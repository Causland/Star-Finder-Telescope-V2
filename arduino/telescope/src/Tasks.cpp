#include <Arduino_FreeRTOS.h>
#include <lib/FreeRTOS-Kernel-v10.5.1\message_buffer.h>

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

  DEBUG_EXIT("createTasks()");
  return true;
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
  /* If the buffers to be provided to the Idle task are declared inside this
  function then they must be declared static - otherwise they will be allocated on
  the stack and so not exists after this function exits. */
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
  
  /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
  state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
  
  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;
  
  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
  /* If the buffers to be provided to the Timer task are declared inside this
  function then they must be declared static - otherwise they will be allocated on
  the stack and so not exists after this function exits. */
  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];
  
  /* Pass out a pointer to the StaticTask_t structure in which the Timer
  task's state will be stored. */
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
  
  /* Pass out the array that will be used as the Timer task's stack. */
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;
  
  /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}