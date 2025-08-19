#include <cstdio>
#include <esp_log.h>
#include <ESP32Servo.h>

#include "PIDController.h"

void PIDController::move()
{
  const auto currUpdateMs{millis()};

  // If we are out of the settling window, do not move but
  // continue to calculate position and velocity for live telemetry
  const bool settled{settlingTimeSec > 0 &&
                     currUpdateMs - targetTimeMs > settlingTimeSec * 1000};
  if (settled)
  {
    servo->stop();
    prevIntegral = 0;
  }

  // Find time between this and last call
  const double deltaS{(currUpdateMs - prevUpdateMs) / 1000.0};

  // Update position
  const double currAngle{servo->measurePosition()};
  const double prevAngle{servo->getPrevPos()};

  // Ignore sporadic jumps greater than max possible speed
  const auto dist{currAngle > prevAngle ? currAngle - prevAngle : 
                                          prevAngle - currAngle};
  if (dist > servo->maxSpeedDps * deltaS) return;

  // Filter the input current angle
  static constexpr double ANGLE_FILTER_FREQ{100}; // Very light filtering. This should be ok from sensor
  const double filteredCurrAngle{prevFilteredCurrAngle + 
                                 (ANGLE_FILTER_FREQ * deltaS / (1.0 + ANGLE_FILTER_FREQ * deltaS)) * 
                                 (currAngle - prevFilteredCurrAngle)};

  // Calculate derivative portion
  static constexpr double VEL_FILTER_FREQ{120};
  const double vel{(filteredCurrAngle - prevFilteredCurrAngle) / deltaS};
  const double filteredVel{prevFilteredVel + 
                           (VEL_FILTER_FREQ * deltaS / (1.0 + VEL_FILTER_FREQ * deltaS)) * 
                           (vel - prevFilteredVel)};

  if (!settled)
  {
    // Calculate error based on filtered position
    double error{filteredCurrAngle - targetAngle};

    // Calculate integral portion
    double integral{prevIntegral + error * deltaS};
    if (integral < -1 * (servo->minSpeedOffsetUs / K_I)) 
        integral = -1 * (servo->minSpeedOffsetUs / K_I);
    else if (integral > servo->minSpeedOffsetUs / K_I)
        integral = servo->minSpeedOffsetUs / K_I;
    
    const double propPortion{error * K_P};
    const double integPortion{integral * K_I};
    const double derivPortion{filteredVel * K_D};
    const int offset{static_cast<int>(propPortion + integPortion + derivPortion)};

    if (esp_log_level_get(TAG) >= ESP_LOG_DEBUG)
    {
      static char buf[128]{};
      std::snprintf(buf, sizeof(buf), "%.3f, %i, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f. %.3f, %i",
                    currUpdateMs / 1000.0,
                    servo->turns,
                    currAngle,
                    filteredCurrAngle,
                    targetAngle,
                    error,
                    propPortion,
                    integPortion,
                    derivPortion,
                    offset);
      ESP_LOGD(TAG, "%s", buf);
    }

    servo->writeMicroseconds(servo->defaultUs + offset);
    
    // Only updated when moving
    prevIntegral = integral;
  }

  // Setup for next function call
  prevFilteredCurrAngle = filteredCurrAngle;
  prevFilteredVel = filteredVel;
  prevUpdateMs = currUpdateMs;  
}