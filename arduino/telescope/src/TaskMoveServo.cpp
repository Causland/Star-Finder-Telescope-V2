#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Tasks.h"
#include "Utils.h"

void taskMoveServo(void* params)
{
  (void) params; // Unused, silence warnings

  DEBUG_ENTER("taskMoveServo()");

  FOREVER
  {
    DEBUG_HEARTBEAT("MoveServo");

    vTaskDelay(1000 * portTICK_PERIOD_MS);
  }

  DEBUG_EXIT("taskMoveServo()");
}