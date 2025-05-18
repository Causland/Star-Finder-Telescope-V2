#ifndef __CUSTOM_SERVO_H__
#define __CUSTOM_SERVO_H__

#include <ESP32Servo.h>

/*!
 * Servo wrapper class to provide some default properties and state to servos.
 */ 
class CustomServo
{
public:

  /// Create a CustomServo with a dedicated output pin and uS range.
  /// @param[in] pinNum the physical pin number.
  /// @param[in] defaultUs the default microseconds for the servo.
  /// @param[in] minUs the minimum microseconds for the servo.
  /// @param[in] maxUs the maximum microseconds for the servo.
  CustomServo(const uint8_t pinNum, const uint16_t defaultUs, 
              const uint16_t minUs, const uint16_t maxUs) :
                pinNum(pinNum), defaultUs(defaultUs), minUs(minUs), maxUs(maxUs) {}

  /// Initializes the default state of the servo and attaches to the underlying resource
  void init()
  {
    servo.attach(pinNum);
    servo.writeMicroseconds(defaultUs);
  }

  virtual ~CustomServo() = default;
  CustomServo(const CustomServo&) = delete;
  CustomServo& operator=(const CustomServo&) = delete;
  CustomServo(CustomServo&&) = delete;
  CustomServo& operator=(CustomServo&&) = delete;

  /// Wrapper function to write a certain microsecond value to the servo object.
  /// @param[in] us microseconds to write.
  void writeMicroseconds(const int& us) { servo.writeMicroseconds(us); }

  /// Stop any motion.
  virtual void stop() = 0;

  /// Measure the current position of the servo.
  /// @return the position of the servo in degrees.
  virtual double measurePosition() = 0;

  double getCurrPos() { return currAngle; }
  double getPrevPos() { return prevAngle; }

  uint8_t pinNum{0}; /// The physical pin number.

  uint16_t defaultUs{0}; /// The default microseconds for the servo.
  uint16_t minUs{0}; /// The minimum microseconds for the servo.
  uint16_t maxUs{UINT16_MAX}; /// The maximum microseconds for the servo.

  double currAngle{0.0}; /// Current angle of the servo. Most recent measurement.
  double prevAngle{0.0}; /// Previous angle of the servo.

protected:
  /// Measure the duty cycle of a pin using a +-100 window around the period.
  /// @param[in] pin the pin to read the duty cycle.
  /// @param[in] periodUs the period of the full cycle in microseconds.
  /// @param[in] average the number of samples to average the duty cycle over.
  /// @param[in] timeoutMs the duration to wait for a valid duty cycle calculation in milliseconds.
  /// @return the duty cycle. -1.0 if invalid.
  static double measureDutyCycle(const int& pin, const int& periodUs, 
                                 const int& average, const int& timeoutMs);
  
  Servo servo; /// The underlying Servo object to control the servo.
};

/// A generic 360 degree continuous servo.
class ContinuousServo : public CustomServo
{
public:
  /// Create a ContinuousServo with a dedicated output pin, uS range, and speed properties.
  /// @param[in] pinNum the physical pin number.
  /// @param[in] defaultUs the default microseconds for the servo.
  /// @param[in] minSpeedOffsetUs the minimum microsecond offset to cause the servo to rotate.
  /// @param[in] maxSpeedOffsetUs the maximum microsecond offset the servo can be set (max speed).
  /// @param[in] maxSpeedDps the calculated maximum speed in deg per second the servo can operate.
  ///                        Used to help filter sporadic jumps in sensors.
  /// @param[in] feedbackPinNum the physical pin number of the feedback sensor.
  ContinuousServo(const uint8_t pinNum, const uint16_t defaultUs,
                   const uint16_t minSpeedOffsetUs, const uint16_t maxSpeedOffsetUs,
                   const uint16_t maxSpeedDps, const uint8_t feedbackPinNum) : 
                    CustomServo(pinNum, defaultUs, defaultUs - maxSpeedOffsetUs, defaultUs + maxSpeedOffsetUs),
                    minSpeedOffsetUs(minSpeedOffsetUs), maxSpeedOffsetUs(maxSpeedOffsetUs), 
                    maxSpeedDps(maxSpeedDps), feedbackPinNum(feedbackPinNum) 
  {
    pinMode(feedbackPinNum, INPUT);
  }

  ~ContinuousServo() override = default;
  ContinuousServo(const ContinuousServo&) = delete;
  ContinuousServo& operator=(const ContinuousServo&) = delete;
  ContinuousServo(ContinuousServo&&) = delete;
  ContinuousServo& operator=(ContinuousServo&&) = delete;

  /// Stop rotation by setting to default microseconds.
  void stop() override { servo.writeMicroseconds(defaultUs); }

  /// Get the true measured angle 0-360 degrees of the sensor. This value is not
  /// adjusted for the number of turns.
  /// @return the measured angle from 0-360 degrees.
  double getMeasuredAngle() { return currMeasuredAngle; }

  uint16_t minSpeedOffsetUs{0}; /// Microsecond offset to cause rotation of servo.
  uint16_t maxSpeedOffsetUs{UINT16_MAX}; /// Microsecond offset to rotate servo at max speed.
  uint16_t maxSpeedDps{UINT16_MAX}; /// Maximum speed of the servo.
  int turns{0}; /// The number of turns the servo has made since startup.

  double currMeasuredAngle{0.0}; /// The real 0-360 measured angle of the servo.
                                 /// CustomServo::currAngle holds the angle adjusted for turns.

protected:
  /// Adjust the number of turns based on the previous and current measured angle.
  /// Uses quadrants to figure out 0-360 crossover
  void calcTurns();

  uint8_t feedbackPinNum{0}; /// The physical pin number of the feedback sensor.

  double prevMeasuredAngle{0.0}; /// The real 0-360 previous measured angle of the servo.
                                 /// CustomServo::prevAngle holds the angle adjusted for turns.
};

/// A ContinuousServo implementation for a Parallax 360 servo.
class Parallax360Servo : public ContinuousServo
{
public:
  /// Create a ContinuousServo with a dedicated output pin, uS range, and speed properties.
  /// @param[in] pinNum the physical pin number.
  /// @param[in] defaultUs the default microseconds for the servo.
  /// @param[in] minSpeedOffsetUs the minimum microsecond offset to cause the servo to rotate.
  /// @param[in] maxSpeedOffsetUs the maximum microsecond offset the servo can be set (max speed).
  /// @param[in] maxSpeedDps the calculated maximum speed in deg per second the servo can operate.
  ///                        Used to help filter sporadic jumps in sensors.
  /// @param[in] feedbackPinNum the physical pin number of the feedback sensor.
  Parallax360Servo(const uint8_t pinNum, const uint16_t defaultUs,
                   const uint16_t minSpeedOffsetUs, const uint16_t maxSpeedOffsetUs,
                   const uint16_t maxSpeedDps, const uint8_t feedbackPinNum) : 
                     ContinuousServo(pinNum, defaultUs,
                                     minSpeedOffsetUs, maxSpeedOffsetUs, 
                                     maxSpeedDps, feedbackPinNum) {}
  
  ~Parallax360Servo() override = default;
  Parallax360Servo(const Parallax360Servo&) = delete;
  Parallax360Servo& operator=(const Parallax360Servo&) = delete;
  Parallax360Servo(Parallax360Servo&&) = delete;
  Parallax360Servo& operator=(Parallax360Servo&&) = delete;

  /// Measure the position of the servo and adjust on turn count
  /// @return the adjusted turn based position
  double measurePosition() override;
};


/// A generic position controlled servo.
class PositionalServo : public CustomServo
{
public:
  /// Create a PositionalServo with a dedicated output pin and uS range.
  /// @param[in] pinNum the physical pin number.
  /// @param[in] defaultUs the default microseconds for the servo.
  /// @param[in] minUs the minimum microseconds for the servo.
  /// @param[in] maxUs the maximum microseconds for the servo.
  /// @param[in] rangeDeg the motion range of the servo in degrees.
  PositionalServo(const uint8_t pinNum,
                  const uint16_t defaultUs, const uint16_t minUs, const uint16_t maxUs,
                  const double& rangeDeg) : CustomServo(pinNum, defaultUs, minUs, maxUs),
                    rangeDeg(rangeDeg), usPerDeg(rangeDeg > 0.0 ? (maxUs - minUs) / rangeDeg : 0.0) {}
  
  ~PositionalServo() override = default;
  PositionalServo(const PositionalServo&) = delete;
  PositionalServo& operator=(const PositionalServo&) = delete;
  PositionalServo(PositionalServo&&) = delete;
  PositionalServo& operator=(PositionalServo&&) = delete;

  /// Stop the servo by writing the current position
  void stop() override { servo.writeMicroseconds(servo.readMicroseconds()); }

  /// Estimate the position of the servo by finding current 
  /// servo microseconds.
  double measurePosition() override
  {
    prevAngle = currAngle;
    currAngle = servo.readMicroseconds() / usPerDeg;
    return currAngle;
  }

  double rangeDeg{0.0}; /// The range of motion of the servo.
  double usPerDeg{0.0}; /// The number of microseconds for each degree of motion in the range.
};

#endif // __CUSTOM_SERVO_H__