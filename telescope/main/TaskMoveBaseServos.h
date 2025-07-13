#ifndef __TASK_MOVE_BASE_SERVOS_H__
#define __TASK_MOVE_BASE_SERVOS_H__

#include <array>

#include "CustomServo.h"
#include "CustomTask.h"
#include "MoveBaseServosCmd.h"
#include "PIDController.h"
#include "Telemetry.h"

class TaskMoveBaseServos : public CustomTask
{
public:
  TaskMoveBaseServos(Telemetry& telemetry, const esp_pthread_cfg_t& threadConfig);

  TaskMoveBaseServos(const TaskMoveBaseServos&) = delete;
  TaskMoveBaseServos(TaskMoveBaseServos&&) = delete;

  ~TaskMoveBaseServos() override = default;

  TaskMoveBaseServos& operator=(const TaskMoveBaseServos&) = delete;
  TaskMoveBaseServos& operator=(TaskMoveBaseServos&&) = delete;

  static double getCurrAz() { return horizPID.prevFilteredCurrAngle; }
  static double getCurrEl() { return vertServo.currAngle; }
  static double getSpeedAz() { return horizPID.prevFilteredVel; }
  static float getTargetAz() { return targetAz; }
  static float getTargetEl() { return targetEl; }

protected:
  void threadLoop() override;

private:
  void processServoUpdate(const std::shared_ptr<MoveBaseServosCmd>& moveServoCmd);

  static constexpr uint8_t VERT_SERVO_PIN{25};
  static constexpr uint16_t VERT_SERVO_MIN_US{860};
  static constexpr uint16_t VERT_SERVO_MAX_US{1490};
  static constexpr double VERT_SERVO_MOTION_RANGE_DEG{90.0};

  static constexpr uint8_t HORIZ_SERVO_PIN{26};
  static constexpr uint8_t HORIZ_SERVO_FEEDBACK_PIN{27};
  static constexpr uint16_t HORIZ_SERVO_STOP_US{1500};
  static constexpr uint16_t HORIZ_SERVO_MIN_SPEED_OFFSET_US{30};
  static constexpr uint16_t HORIZ_SERVO_MAX_SPEED_OFFSET_US{220};
  static constexpr uint16_t HORIZ_SERVO_MAX_SPEED_DPS{840};
  static constexpr uint16_t HORIZ_SERVO_DEFAULT_US{HORIZ_SERVO_STOP_US};
  static constexpr uint16_t HORIZ_SERVO_REFRESH_RATE_MS{10};

  static PositionalServo vertServo;
  static std::shared_ptr<Parallax360Servo> horizServo;
  static PIDController horizPID;
  static float targetAz;
  static float targetEl;
};

#endif // __TASK_MOVE_BASE_SERVOS_H__