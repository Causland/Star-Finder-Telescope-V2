#ifndef __TASK_FOCUS_H__
#define __TASK_FOCUS_H__

#include <chrono>

#include "CustomServo.h"
#include "CustomTask.h"
#include "FocusCmd.h"
#include "Telemetry.h"

class TaskFocus : public CustomTask
{
public:
  TaskFocus(Telemetry& telemetry, const esp_pthread_cfg_t& threadConfig);

  TaskFocus(const TaskFocus&) = delete;
  TaskFocus(TaskFocus&&) = delete;

  ~TaskFocus() override = default;

  TaskFocus& operator=(const TaskFocus&) = delete;
  TaskFocus& operator=(TaskFocus&&) = delete;

protected:
  void threadLoop() override;

private:
  void rotateFocus(const float& degrees);

  static constexpr uint8_t FOCUS_SERVO_PIN{32};
  static constexpr uint16_t FOCUS_SERVO_STOP_US{1500};
  static constexpr uint16_t FOCUS_SERVO_MIN_SPEED_OFFSET_US{50};
  static constexpr uint16_t FOCUS_SERVO_MAX_SPEED_OFFSET_US{500};
  static constexpr uint16_t FOCUS_SERVO_MIN_SPEED_DPS{27};
  static constexpr uint16_t FOCUS_SERVO_MAX_SPEED_DPS{273};
  static constexpr uint16_t FOCUS_SERVO_DEFAULT_US{FOCUS_SERVO_STOP_US};

  ContinuousServo focusServo{FOCUS_SERVO_PIN, FOCUS_SERVO_STOP_US, 
                             FOCUS_SERVO_MIN_SPEED_OFFSET_US, FOCUS_SERVO_MAX_SPEED_OFFSET_US,
                             FOCUS_SERVO_MAX_SPEED_DPS};
};

#endif // __TASK_FOCUS_H__