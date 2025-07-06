#ifndef __PID_CONTROLLER_H__
#define __PID_CONTROLLER_H__

#include <esp_log.h>
#include <memory>

#include "CustomServo.h"

/// A basic implementation of a generic PID controller for a continuous servo.
class PIDController
{
public:

  /// Create a PIDController with the provided ContinousServo and PID constants.
  /// @param[in] servo a pointer to a ContinuousServo object for position, properties, and control.
  ///                  This pointer is moved into the PIDController.
  /// @param[in] settlingTimeSec the duration in seconds in which to control the servo before stopping PID.
  /// @param[in] P the P constant.
  /// @param[in] I the I constant.
  /// @param[in] D the D constant.
  PIDController(std::unique_ptr<ContinuousServo>& servo, const int& settlingTimeSec=-1,
                const double& P=0.0, const double& I=0.0, const double& D=0.0) : 
                  servo(std::move(servo)), settlingTimeSec(settlingTimeSec), K_P(P), K_I(I), K_D(D) 
  {
    if (servo == nullptr) 
    {
      ESP_LOGE(TAG, "Servo cannot be null for PID controller");
      while (true) { ; }
    }
  }

  /// Update the PID controller and move the servo. Applies filtering to input and velocity before
  /// calculating microsecond offset for motion.
  void move();

  /// Update the target for the controller.
  /// @param[in] newTargetDeg the new target in degrees.
  void updateTarget(const double& newTargetDeg) 
  { 
    targetTimeMs = millis();
    targetAngle = newTargetDeg;
    prevIntegral = 0.0; 
  }

  double prevFilteredCurrAngle{0.0}; /// The previous filtered angle of the servo.
  double prevFilteredVel{0.0}; /// The previous filtered velocity of the servo.

private:
  std::unique_ptr<ContinuousServo> servo; /// A pointer to the ContinousServo for position and control.

  int settlingTimeSec{-1}; /// The duration in seconds in which to let the PID controller operate before stopping.
                           /// Important for stopping drift of servo due to I.
                           
  double K_P{0.0}; /// The proportional constant.
  double K_I{0.0}; /// The integration constant.
  double K_D{0.0}; /// The derivative constant.

  unsigned long prevUpdateMs{millis()}; /// The last time the move() function was called.
  double prevIntegral{0.0}; /// The previous integral value.

  unsigned long targetTimeMs{millis()}; /// Time of the last target update.
  double targetAngle{0.0}; /// The target angle in degrees.

  static constexpr const char* TAG{"PIDController"}; /// The tag for logging.
};

#endif // __PID_CONTROLLER_H__