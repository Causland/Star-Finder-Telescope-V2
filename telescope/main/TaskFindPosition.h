#ifndef __TASK_FIND_POSITION_H__
#define __TASK_FIND_POSITION_H__

#include <array>
#include <chrono>

#include "CustomTask.h"
#include "Telemetry.h"

class TaskFindPosition : public CustomTask
{
public:
  static constexpr std::chrono::seconds GPS_RETRY_PERIOD{5};
  static constexpr size_t NUM_REQUIRED_SUCCESSFUL_READINGS{3};

  TaskFindPosition(Telemetry& telemetry, const esp_pthread_cfg_t& threadConfig);

  TaskFindPosition(const TaskFindPosition&) = delete;
  TaskFindPosition(TaskFindPosition&&) = delete;

  ~TaskFindPosition() override = default;

  TaskFindPosition& operator=(const TaskFindPosition&) = delete;
  TaskFindPosition& operator=(TaskFindPosition&&) = delete;

  static float getGPSLat() { return gpsLat; }
  static float getGPSLon() { return gpsLon; }
  static float getGPSAlt() { return gpsAlt; }

protected:
  void threadLoop() override;

private:
  static float gpsLat; ///< Latitude of the GPS position
  static float gpsLon; ///< Longitude of the GPS position
  static float gpsAlt; ///< Altitude of the GPS position in meters

  size_t readings{}; ///< Number of successful GPS readings
};

#endif // __TASK_FIND_POSITION_H__