#include <Arduino.h>

#include "CustomServo.h"

double CustomServo::measureDutyCycle(const int& pin, const int& periodUs, 
  const int& average, const int& timeoutMs)
{
  const auto startTime{millis()};
  auto sampleCount{0};
  double dcSum{0.0};

  while(millis() - startTime < timeoutMs)
  {
    const auto tHigh{pulseIn(pin, HIGH, 5000)};
    const auto tLow{pulseIn(pin, LOW, 5000)};
    const auto tCycle{tHigh + tLow};
    if (tCycle > periodUs - 100 && tCycle < periodUs + 100)
    {
      dcSum += static_cast<double>(tHigh) / tCycle;
      ++sampleCount;
      if (sampleCount == average)
      {
        return dcSum / average;
      }
    }
  }
  return -1.0;
}

void ContinuousServo::calcTurns()
{
  // Use quadrants to figure out turn
  if (prevMeasuredAngle >= 270.0 && currMeasuredAngle <= 90.0)
  {
    ++turns;
  }
  else if (prevMeasuredAngle <= 90.0 && currMeasuredAngle >= 270.0) 
  {
    --turns;
  }
}

double Parallax360Servo::measurePosition()
{
  static constexpr double DC_MIN{0.029}; /// Min duty cycle
  static constexpr double DC_MAX{0.971}; /// Max duty cycle
  static constexpr int CYCLE_PERIOD_US{1100}; /// The approx. period of one complete cycle

  const double dc{measureDutyCycle(feedbackPinNum,
                                   CYCLE_PERIOD_US,
                                   3,
                                   10)};
  if (dc < 0.0) return prevAngle; // The calculation failed, nothing to do anymore

  // Clamp measured angle into the 0 - 360 range
  prevMeasuredAngle = currMeasuredAngle;
  currMeasuredAngle = ((dc - DC_MIN) * 360.0) / (DC_MAX - DC_MIN);
  if (currMeasuredAngle < 0 || currMeasuredAngle >= 360.0) currMeasuredAngle = 0;

  calcTurns();
  
  prevAngle = currAngle;
  currAngle = 360.0 * turns + currMeasuredAngle;
  return currAngle;
}