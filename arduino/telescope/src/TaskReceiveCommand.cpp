#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Tasks.h"
#include "Utils.h"

void taskReceiveCommand(void* params)
{
  (void) params; // Unused, silence warnings

  DEBUG_ENTER("taskReceiveCommand()");

  FOREVER
  {
    DEBUG_HEARTBEAT("ReceiveCommand");

    vTaskDelay(1000 * portTICK_PERIOD_MS);
  }

  DEBUG_EXIT("taskReceiveCommand()");
}