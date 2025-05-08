#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Commands.h"
#include "Tasks.h"
#include "Telemetry.h"
#include "Utils.h"
#include "Wifi.h"

/// Global task objects used by tasks
void* gTaskParams[NUM_TASKS]{0};
TaskHandle_t gTaskHandles[NUM_TASKS]{0};

StackType_t gTaskStack[NUM_TASKS][TASK_STACK_DEPTH]{0};
StaticTask_t gTaskTCB[NUM_TASKS]{0};

uint8_t gMsgBufferStorage[NUM_TASKS][MAX_CMD_SIZE]{0};
StaticMessageBuffer_t gMsgBuffer[NUM_TASKS]{0};
MessageBufferHandle_t gMsgBufferHandles[NUM_TASKS]{0};

Telemetry gTelemetry{};

/// Global Wifi objects used by tasks
Wifi gWifi;

/// Parameter objects used by tasks
CollectTelemetryParams gCollectTelemetryParams{};
RecvCmdParams gRecvCmdParams{};
MoveBaseServoParams gMoveBaseServoParams{};
PlanTrajectoryParams gPlanTrajectoryParams{};

// Create GPS objects
/// @todo: Move to permanent location in the code
float gpsLat{0.0};
float gpsLon{0.0};
float gpsAlt{0.0};

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
  bool allocStatus = createCmdBuffers(gMsgBufferHandles, gMsgBuffer, gMsgBufferStorage);
  if (!allocStatus)
  {
    DEBUG_PRINTLN("Failed to allocate command buffers! Stopping...");
    while (true) { ; } // There was a failure allocating command buffers. Block and loop forever
  }

  /// @todo: Add GPS object creation here
  gTelemetry.registerTelemFieldGPSLat(&gpsLat);
  gTelemetry.registerTelemFieldGPSLon(&gpsLon);
  gTelemetry.registerTelemFieldGPSAlt(&gpsAlt);

  // Provide parameters to tasks which need them
  gCollectTelemetryParams.telemSender = &gWifi.telemSender;
  gCollectTelemetryParams.msgBufferHandle = gMsgBufferHandles[TASK_COLLECT_TELEMETRY];
  gCollectTelemetryParams.telemetry = &gTelemetry;
  gTaskParams[TASK_COLLECT_TELEMETRY] = &gCollectTelemetryParams;

  gRecvCmdParams.cmdReceiver = &gWifi.cmdReceiver;
  gRecvCmdParams.taskHandles = gTaskHandles;
  gRecvCmdParams.msgBufferHandles = gMsgBufferHandles;
  gRecvCmdParams.telemetry = &gTelemetry;
  gTaskParams[TASK_RECEIVE_COMMAND] = &gRecvCmdParams;

  gMoveBaseServoParams.msgBufferHandle = gMsgBufferHandles[TASK_MOVE_BASE_SERVOS];
  gMoveBaseServoParams.telemetry = &gTelemetry;
  gTaskParams[TASK_MOVE_BASE_SERVOS] = &gMoveBaseServoParams;

  gPlanTrajectoryParams.msgBufferHandle = gMsgBufferHandles[TASK_PLAN_TRAJECTORY];
  gPlanTrajectoryParams.moveCmdBufferHandle = gMsgBufferHandles[TASK_MOVE_BASE_SERVOS];
  gPlanTrajectoryParams.telemetry = &gTelemetry;
  gTaskParams[TASK_PLAN_TRAJECTORY] = &gPlanTrajectoryParams;

  BaseType_t result = createTasks(gTaskParams, gTaskHandles,
                                  gTaskStack, gTaskTCB);
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
