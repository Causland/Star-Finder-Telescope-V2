#include <Arduino.h>

#include "Commands.h"
#include "GPS.h"
#include "Tasks.h"
#include "Telemetry.h"
#include "Utils.h"
#include "WiFiWrapper.h"

/// Global task objects used by tasks
void* gTaskParams[NUM_TASKS]{0};
TaskHandle_t gTaskHandles[NUM_TASKS]{0};
StackType_t gTaskStack[NUM_TASKS][TASK_STACK_DEPTH]{0};
StaticTask_t gTaskTCB[NUM_TASKS]{0};

EventGroupHandle_t gStartEventGroup;

MessageBufferHandle_t gMsgBufferHandles[NUM_TASKS]{0};
Telemetry gTelemetry{};

GPS gGPS{};

/// Global Wifi objects used by tasks
WiFiWrapper gWifi;

/// Parameter objects used by tasks
CollectTelemetryParams gCollectTelemetryParams{};
RecvCmdParams gRecvCmdParams{};
MoveBaseServoParams gMoveBaseServoParams{};
PlanTrajectoryParams gPlanTrajectoryParams{};
ControlCameraParams gControlCameraParams{};

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

  // Initialize GPS module
  if (!gGPS.init())
  {
    DEBUG_PRINTLN("Failed to initialize GPS module! Stopping...");
    while (true) { ; } // There was a failure starting the GPS module. Block and loop forever
  }
  if (!gGPS.getData())
  {
    DEBUG_PRINTLN("Failed to get GPS data! Using defaults");
  }
  gGPS.deinit();

  // Register telemetry fields with the telemetry object
  gTelemetry.registerTelemFieldGPSLat(&gGPS.gpsLat);
  gTelemetry.registerTelemFieldGPSLon(&gGPS.gpsLon);
  gTelemetry.registerTelemFieldGPSAlt(&gGPS.gpsAlt);

  // Create task command buffers
  bool allocStatus = createCmdBuffers(gMsgBufferHandles);
  if (!allocStatus)
  {
    DEBUG_PRINTLN("Failed to allocate command buffers! Stopping...");
    while (true) { ; } // There was a failure allocating command buffers. Block and loop forever
  }

  // Create the event group to signal to tasks to begin their forever loops
  gStartEventGroup = xEventGroupCreate();

  // Provide parameters to tasks which need them
  gCollectTelemetryParams.telemSender = &gWifi.telemSender;
  gCollectTelemetryParams.msgBufferHandle = gMsgBufferHandles[TASK_COLLECT_TELEMETRY];
  gCollectTelemetryParams.telemetry = &gTelemetry;
  gCollectTelemetryParams.startEvent = gStartEventGroup;
  gTaskParams[TASK_COLLECT_TELEMETRY] = &gCollectTelemetryParams;

  gRecvCmdParams.cmdReceiver = &gWifi.cmdReceiver;
  gRecvCmdParams.taskHandles = gTaskHandles;
  gRecvCmdParams.msgBufferHandles = gMsgBufferHandles;
  gRecvCmdParams.telemetry = &gTelemetry;
  gRecvCmdParams.startEvent = gStartEventGroup;
  gTaskParams[TASK_RECEIVE_COMMAND] = &gRecvCmdParams;

  gMoveBaseServoParams.msgBufferHandle = gMsgBufferHandles[TASK_MOVE_BASE_SERVOS];
  gMoveBaseServoParams.telemetry = &gTelemetry;
  gMoveBaseServoParams.startEvent = gStartEventGroup;
  gTaskParams[TASK_MOVE_BASE_SERVOS] = &gMoveBaseServoParams;

  gPlanTrajectoryParams.msgBufferHandle = gMsgBufferHandles[TASK_PLAN_TRAJECTORY];
  gPlanTrajectoryParams.moveCmdBufferHandle = gMsgBufferHandles[TASK_MOVE_BASE_SERVOS];
  gPlanTrajectoryParams.telemetry = &gTelemetry;
  gPlanTrajectoryParams.startEvent = gStartEventGroup;
  gTaskParams[TASK_PLAN_TRAJECTORY] = &gPlanTrajectoryParams;

  gControlCameraParams.cameraSender = &gWifi.cameraSender;
  gControlCameraParams.msgBufferHandle = gMsgBufferHandles[TASK_CONTROL_CAMERA];
  gControlCameraParams.telemetry = &gTelemetry;
  gControlCameraParams.startEvent = gStartEventGroup;
  gTaskParams[TASK_CONTROL_CAMERA] = &gControlCameraParams;

  bool result = createTasks(gTaskParams, gTaskHandles,
                            gTaskStack, gTaskTCB);
  if (!result)
  {
    DEBUG_PRINTLN("Failed to create all tasks! Stopping...");
    while (true) { ; } // There was a failure creating tasks. Block and loop forever
  }

  xEventGroupSetBits(gStartEventGroup, BIT0); // Signal tasks to start

  DEBUG_EXIT("setup()");
}

void loop()
{
  // Nothing to do here. Everything is managed by scheduler
}
