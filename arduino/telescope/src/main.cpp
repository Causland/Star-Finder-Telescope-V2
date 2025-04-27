#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Tasks.h"
#include "Utils.h"

// Global variables and constants
void* gTaskParams[NUM_TASKS]{0};
TaskHandle_t* gTaskHandles[NUM_TASKS]{0};

void setup()
{
  DEBUG_INIT;
  DEBUG_ENTER("setup()");

  BaseType_t result = createTasks(gTaskParams, gTaskHandles);
  if (result != pdPASS)
  {
    DEBUG_PRINTLN("Failed to create all tasks! Stopping...");
    while (true) { ; } // There was a failure creating tasks. Block and loop forever
  }

  // Once tasks are created, scheduler takes over running the program
  vTaskStartScheduler();

  DEBUG_EXIT("setup()"); // This should never be reached
}

void loop()
{
  // Nothing to do here. Everything is managed by scheduler
}
