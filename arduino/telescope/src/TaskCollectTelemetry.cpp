#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Tasks.h"
#include "Utils.h"

void taskCollectTelemetry(void* params)
{
  (void) params; // Unused, silence warnings

  DEBUG_ENTER("taskCollectTelemetry()");

  FOREVER
  {
    DEBUG_HEARTBEAT("CollectTelemetry");

    vTaskDelay(1000 * portTICK_PERIOD_MS);
  }

  DEBUG_EXIT("taskCollectTelemetry()");
}