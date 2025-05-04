#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Commands.h"
#include "Tasks.h"
#include "Utils.h"
#include "Wifi.h"

/// Global task objects used by tasks
void* gTaskParams[NUM_TASKS]{0};
TaskHandle_t gTaskHandles[NUM_TASKS]{0};
MessageBufferHandle_t gMsgBufferHandles[NUM_TASKS]{0};


/// Global Wifi objects used by tasks
Wifi gWifi;

/// Parameter objects used by tasks
CollectTelemetryParams gCollectTelemetryParams{};
RecvCmdParams gRecvCmdParams{};
MoveBaseServoParams gMoveBaseServoParams{};
PlanTrajectoryParams gPlanTrajectoryParams{};

void setup()
{
  DEBUG_INIT;
  DEBUG_ENTER("setup()");

  // Configure wifi access point
  if (!gWifi.init())
  {
    DEBUG_PRINTLN("Failed to start WiFi interfaces! Stopping...");
    while (true) { ; } // There was a failure start one of the Wifi interfaces. Block and loop forever
  }

  // Create task command buffers
  bool allocStatus = createCmdBuffers(gMsgBufferHandles);
  if (!allocStatus)
  {
    DEBUG_PRINTLN("Failed to allocate command buffers! Stopping...");
    while (true) { ; } // There was a failure allocating command buffers. Block and loop forever
  }

  // Provide parameters to tasks which need them
  gCollectTelemetryParams.telemSender = &gWifi.telemSender;
  gCollectTelemetryParams.msgBufferHandle = gMsgBufferHandles[TASK_COLLECT_TELEMETRY];
  gTaskParams[TASK_COLLECT_TELEMETRY] = &gCollectTelemetryParams;

  gRecvCmdParams.cmdReceiver = &gWifi.cmdReceiver;
  gRecvCmdParams.taskHandles = gTaskHandles;
  gRecvCmdParams.msgBufferHandles = gMsgBufferHandles;
  gTaskParams[TASK_RECEIVE_COMMAND] = &gRecvCmdParams;

  gMoveBaseServoParams.msgBufferHandle = gMsgBufferHandles[TASK_MOVE_BASE_SERVOS];
  gTaskParams[TASK_MOVE_BASE_SERVOS] = &gMoveBaseServoParams;

  gPlanTrajectoryParams.msgBufferHandle = gMsgBufferHandles[TASK_PLAN_TRAJECTORY];
  gPlanTrajectoryParams.moveCmdBufferHandle = gMsgBufferHandles[TASK_MOVE_BASE_SERVOS];
  gTaskParams[TASK_PLAN_TRAJECTORY] = &gPlanTrajectoryParams;

  BaseType_t result = createTasks(gTaskParams, gTaskHandles);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create all tasks! Stopping...");
    while (true) { ; } // There was a failure creating tasks. Block and loop forever
  }

  // Once tasks are created, scheduler takes over running the program
  vTaskStartScheduler();

  DEBUG_EXIT("setup()"); // This should never be reached
}

void loop()
{
  // Nothing to do here. Everything is managed by scheduler
}
