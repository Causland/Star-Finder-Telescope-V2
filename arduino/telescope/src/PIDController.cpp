#include "PIDController.h"

void PIDController::move()
{
  const auto currUpdateMs{millis()};

  // If we are out of the settling window, do not move
  if (settlingTimeSec > 0 &&
      currUpdateMs - targetTimeMs > settlingTimeSec * 1000)
  {
    servo->stop();
    return;
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

  // Calculate error based on filtered position
  double error{filteredCurrAngle - targetAngle};

  // Calculate integral portion
  double integral{prevIntegral + error * deltaS};
  if (integral < -1 * (servo->minSpeedOffsetUs / K_I)) 
      integral = -1 * (servo->minSpeedOffsetUs / K_I);
  else if (integral > servo->minSpeedOffsetUs / K_I)
      integral = servo->minSpeedOffsetUs / K_I;

  // Calculate derivative portion
  static constexpr double VEL_FILTER_FREQ{120};
  const double vel{(filteredCurrAngle - prevFilteredCurrAngle) / deltaS};
  const double filteredVel{prevFilteredVel + 
                           (VEL_FILTER_FREQ * deltaS / (1.0 + VEL_FILTER_FREQ * deltaS)) * 
                           (vel - prevFilteredVel)};
  
  const double propPortion{error * K_P};
  const double integPortion{integral * K_I};
  const double derivPortion{filteredVel * K_D};
  const int offset{propPortion + integPortion + derivPortion};

#ifdef DEBUG_PID
  static char buf[128]{};
  static char str[16]{};
  dtostrf(currUpdateMs / 1000.0, 0, 3, str); strncpy(buf, str, 15); strcat(buf, ", ");
  itoa(servo->turns, str, 10);               strncat(buf, str, 15); strcat(buf, ", ");
  dtostrf(currAngle, 0, 3, str);             strncat(buf, str, 15); strcat(buf, ", ");
  dtostrf(filteredCurrAngle, 0, 3, str);     strncat(buf, str, 15); strcat(buf, ", ");
  dtostrf(targetAngle, 0, 3, str);           strncat(buf, str, 15); strcat(buf, ", ");
  dtostrf(error, 0, 3, str);                 strncat(buf, str, 15); strcat(buf, ", ");
  dtostrf(propPortion, 0, 3, str);           strncat(buf, str, 15); strcat(buf, ", ");
  dtostrf(integPortion, 0, 3, str);          strncat(buf, str, 15); strcat(buf, ", ");
  dtostrf(derivPortion, 0, 3, str);          strncat(buf, str, 15); strcat(buf, ", ");
  itoa(offset, str, 10);                     strncat(buf, str, 15);
  DEBUG_PRINTLN("" + String(buf));
#endif

  servo->writeMicroseconds(servo->defaultUs + offset);

  // Setup for next function call
  prevFilteredCurrAngle = filteredCurrAngle;
  prevIntegral = integral;
  prevFilteredVel = filteredVel;
  prevUpdateMs = currUpdateMs;  
}