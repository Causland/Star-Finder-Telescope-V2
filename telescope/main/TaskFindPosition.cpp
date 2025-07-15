#include <esp_log.h>

#include "GPS.h"
#include "TaskFindPosition.h"

float TaskFindPosition::gpsLat{};
float TaskFindPosition::gpsLon{};
float TaskFindPosition::gpsAlt{};

TaskFindPosition::TaskFindPosition(Telemetry& telemetry,
                                   const esp_pthread_cfg_t& threadConfig) :
                                    CustomTask(threadConfig)
{
  telemetry.registerTelemFieldGPSLatCB(getGPSLat);
  telemetry.registerTelemFieldGPSLonCB(getGPSLon);
  telemetry.registerTelemFieldGPSAltCB(getGPSAlt);
}

void TaskFindPosition::threadLoop()
{
  // Create the GPS object here in function scope so that the interface is
  // torn down when the position is found
  GPS gps;

  while (!exitFlag)
  {
    // Get latest position reading and compute average
    if (gps.findPosition())
    {
      float totLat = gpsLat * readings;
      float totLon = gpsLon * readings;
      float totAlt = gpsAlt * readings;

      ++readings;

      float lat;
      float lon;
      float alt;
      gps.getPosition(lat, lon, alt);
      ESP_LOGI(cfg.thread_name, "GPS reading #%d: %f, %f, %f",
                                readings, lat, lon, alt);

      gpsLat = (lat + totLat) / readings;
      gpsLon = (lon + totLon) / readings;
      gpsAlt = (alt + totAlt) / readings;      
    }

    if (readings >= NUM_REQUIRED_SUCCESSFUL_READINGS)
    {
      ESP_LOGI(cfg.thread_name, "Final GPS position: %f, %f, %f",
                                gpsLat, gpsLon, gpsAlt);
      exitFlag = true;
      break;
    }

    std::this_thread::sleep_for(GPS_RETRY_PERIOD);
  }
}