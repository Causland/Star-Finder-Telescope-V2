
#include <esp_log.h>

#include "TaskMoveBaseServos.h"
#include "Tasks.h"

PositionalServo TaskMoveBaseServos::vertServo
        {VERT_SERVO_PIN, (VERT_SERVO_MIN_US + VERT_SERVO_MAX_US) / 2, 
         VERT_SERVO_MIN_US, VERT_SERVO_MAX_US, VERT_SERVO_MOTION_RANGE_DEG};
std::shared_ptr<Parallax360Servo> TaskMoveBaseServos::horizServo{};
std::unique_ptr<PIDController> TaskMoveBaseServos::horizPID{};
float TaskMoveBaseServos::targetAz{};
float TaskMoveBaseServos::targetEl{};

TaskMoveBaseServos::TaskMoveBaseServos(Telemetry& telemetry, 
                                       const esp_pthread_cfg_t& threadConfig) :
                                        CustomTask(threadConfig)
{
  horizServo = std::make_shared<Parallax360Servo>(
                HORIZ_SERVO_PIN, HORIZ_SERVO_STOP_US, 
                HORIZ_SERVO_MIN_SPEED_OFFSET_US, HORIZ_SERVO_MAX_SPEED_OFFSET_US,
                HORIZ_SERVO_MAX_SPEED_DPS, HORIZ_SERVO_FEEDBACK_PIN);
  if (!horizServo)
  {
    ESP_LOGE(cfg.thread_name, "Failed to create horizontal servo");
    return;
  }

  horizPID = std::make_unique<PIDController>(horizServo, 10, 1.275, 3.0, 0.425);
  if (!horizPID)
  {
    ESP_LOGE(cfg.thread_name, "Failed to create horizontal servo controller");
    return;
  }

  telemetry.registerTelemFieldCurrAzCB(getCurrAz);
  telemetry.registerTelemFieldCurrElCB(getCurrEl);
  telemetry.registerTelemFieldSpeedAzCB(getSpeedAz);
  telemetry.registerTelemFieldTargetAzCB(getTargetAz);
  telemetry.registerTelemFieldTargetElCB(getTargetEl);
}

void TaskMoveBaseServos::threadLoop()
{
  if (!horizServo)
  {
    ESP_LOGE(cfg.thread_name, "Horiz servo is nullptr");
    return;
  }
  if (!horizPID)
  {
    ESP_LOGE(cfg.thread_name, "Horiz servo controller is nullptr");
    return;
  }

  // Setup servos before starting forever loop
  vertServo.init();
  horizServo->init();
  horizPID->updateTarget(20);

  while (!exitFlag)
  {
    // Wait for a signal or timeout. We 
    std::unique_lock<std::mutex> lk(cmdMutex);
    cv.wait_for(lk, std::chrono::milliseconds(HORIZ_SERVO_REFRESH_RATE_MS));
    
    if (exitFlag) break;

    if (!cmdQueue.empty())
    {
      // Get the command from the queue
      auto cmd = std::move(cmdQueue.front());
      cmdQueue.pop();
      lk.unlock();

      if (!cmd)
      {
        ESP_LOGE(cfg.thread_name, "Received null command!");
      }
      else
      {
        if (cmd->id == Command::CMD_MOVE_BASE_SERVOS)
        {
          auto moveServoCmd = std::static_pointer_cast<MoveBaseServosCmd>(cmd);

          ESP_LOGD(cfg.thread_name, "Received Move Servo Command: %f, %f",
                                    moveServoCmd->az, moveServoCmd->el);

          processServoUpdate(moveServoCmd);
        }
      }
    }

    // Update the position of the vertical
    vertServo.measurePosition();
    
    // Move the horizontal servo using the PID controller. This is called every
    // cycle of the task. If the horizontal servo has settled, this will only
    // update position and velocity measurements
    horizPID->move();
  }
}

void TaskMoveBaseServos::processServoUpdate(const std::shared_ptr<MoveBaseServosCmd>& moveServoCmd)
{
  targetAz = moveServoCmd->az;
  targetEl = moveServoCmd->el;

  // Move the vertical servo to the specified angle
  int numUs{static_cast<int>(moveServoCmd->el * vertServo.usPerDeg + vertServo.minUs)};
  if (numUs < vertServo.minUs) numUs = vertServo.minUs;
  else if (numUs > vertServo.maxUs) numUs = vertServo.maxUs;
  vertServo.writeMicroseconds(numUs);

  // Move the horizontal servo to the specified angle
  double targetAngle{moveServoCmd->az};

  // Calculate the next target based on the current real angle adjusted for turns
  horizServo->measurePosition();
  const auto measuredAngle{horizServo->getMeasuredAngle()};
  const auto& turns{horizServo->turns};
  if (targetAngle > measuredAngle && 
      targetAngle - measuredAngle > 180.0)
  {
    horizPID->updateTarget(targetAngle + 360.0 * (turns - 1));
  }
  else if (measuredAngle > targetAngle &&
            measuredAngle - targetAngle > 180)
  {
    horizPID->updateTarget(targetAngle + 360.0 * (turns + 1));
  }
  else
  {
    horizPID->updateTarget(targetAngle + 360.0 * turns);
  }
}