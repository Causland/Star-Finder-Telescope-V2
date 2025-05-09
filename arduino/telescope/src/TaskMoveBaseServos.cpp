#include <Arduino_FreeRTOS.h>
#include <lib/FreeRTOS-Kernel-v10.5.1\message_buffer.h>
#include <Arduino.h>

#include "Commands.h"
#include "Tasks.h"
#include "Utils.h"
#include "CustomServo.h"
#include "PIDController.h"

// Define servo constants
static constexpr uint8_t VERT_SERVO_PIN{9};
static constexpr uint16_t VERT_SERVO_MIN_US{860};
static constexpr uint16_t VERT_SERVO_MAX_US{1490};
static constexpr double VERT_SERVO_MOTION_RANGE_DEG{90.0};

static constexpr uint8_t HORIZ_SERVO_PIN{10};
static constexpr uint8_t HORIZ_SERVO_FEEDBACK_PIN{11};
static constexpr uint16_t HORIZ_SERVO_STOP_US{1500};
static constexpr uint16_t HORIZ_SERVO_MIN_SPEED_OFFSET_US{30};
static constexpr uint16_t HORIZ_SERVO_MAX_SPEED_OFFSET_US{220};
static constexpr uint16_t HORIZ_SERVO_MAX_SPEED_DPS{840};
static constexpr uint16_t HORIZ_SERVO_DEFAULT_US{HORIZ_SERVO_STOP_US};
static constexpr uint16_t HORIZ_SERVO_REFRESH_RATE_MS{10};

// File local variables
MoveServoCmd_t moveServoCmd{0.0, 0.0};
PositionalServo vertServo{VERT_SERVO_PIN, (VERT_SERVO_MIN_US + VERT_SERVO_MAX_US) / 2, 
                          VERT_SERVO_MIN_US, VERT_SERVO_MAX_US, VERT_SERVO_MOTION_RANGE_DEG};
Parallax360Servo horizServo{HORIZ_SERVO_PIN, HORIZ_SERVO_STOP_US, 
                            HORIZ_SERVO_MIN_SPEED_OFFSET_US, HORIZ_SERVO_MAX_SPEED_OFFSET_US,
                            HORIZ_SERVO_MAX_SPEED_DPS, HORIZ_SERVO_FEEDBACK_PIN};
PIDController horizPID{&horizServo, 5, 1.275, 3.0, 0.425};
float targetAz{0.0};
float targetEl{0.0};

void taskMoveBaseServos(void* params)
{
  DEBUG_ENTER("taskBaseMoveServos()");

  MessageBufferHandle_t msgBufferHandle = static_cast<MoveBaseServoParams*>(params)->msgBufferHandle;
  Telemetry* telemetry = static_cast<MoveBaseServoParams*>(params)->telemetry;

  // Register task telemetry
  telemetry->registerTelemFieldCurrAz(&horizPID.prevFilteredCurrAngle);
  telemetry->registerTelemFieldCurrEl(&vertServo.currAngle);
  telemetry->registerTelemFieldSpeedAz(&horizPID.prevFilteredVel);
  telemetry->registerTelemFieldTargetAz(&targetAz);
  telemetry->registerTelemFieldTargetEl(&targetEl);

  vertServo.init();
  horizServo.init();

  horizPID.updateTarget(20);

  FOREVER
  {
    DEBUG_HEARTBEAT("MoveServo");

    // Check if there is a new command in the message buffer. This will update
    // the target angle for both servos. Do not wait for a command
    size_t bytesRead = xMessageBufferReceive(msgBufferHandle, reinterpret_cast<uint8_t*>(&moveServoCmd), 
                                             sizeof(MoveServoCmd_t), 0);
    if (bytesRead == sizeof(MoveServoCmd_t))
    {
      DEBUG_PRINTLN("Received Move Servo Command: " + String(moveServoCmd.az) + ", " + 
                    String(moveServoCmd.el));
      targetAz = moveServoCmd.az;
      targetEl = moveServoCmd.el;

      // Move the vertical servo to the specified angle
      int numUs{moveServoCmd.el * vertServo.usPerDeg + vertServo.minUs};
      if (numUs < vertServo.minUs) numUs = vertServo.minUs;
      else if (numUs > vertServo.maxUs) numUs = vertServo.maxUs;
      vertServo.writeMicroseconds(numUs);

      // Move the horizontal servo to the specified angle
      double targetAngle{moveServoCmd.az};

      // Calculate the next target based on the current real angle adjusted for turns
      horizServo.measurePosition();
      const auto measuredAngle{horizServo.getMeasuredAngle()};
      const auto& turns{horizServo.turns};
      if (targetAngle > measuredAngle && 
          targetAngle - measuredAngle > 180.0)
      {
        horizPID.updateTarget(targetAngle + 360.0 * (turns - 1));
      }
      else if (measuredAngle > targetAngle &&
                measuredAngle - targetAngle > 180)
      {
        horizPID.updateTarget(targetAngle + 360.0 * (turns + 1));
      }
      else
      {
        horizPID.updateTarget(targetAngle + 360.0 * turns);
      }
    }

    // Update the position of the vertical servo
    vertServo.measurePosition();
    
    // Move the horizontal servo using the PID controller. This is called every
    // cycle of the task
    horizPID.move();

    // Delay for the refresh rate of the horizontal servo
    vTaskDelay(pdMS_TO_TICKS(HORIZ_SERVO_REFRESH_RATE_MS));
  }

  DEBUG_EXIT("taskBaseMoveServos()");
}