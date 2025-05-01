#include <Arduino_FreeRTOS.h>
#include <lib/FreeRTOS-Kernel-v10.5.1\message_buffer.h>
#include <Arduino.h>

#include "Commands.h"
#include "Tasks.h"
#include "Utils.h"

void taskMoveServo(void* params)
{
  MessageBufferHandle_t msgBufferHandle = static_cast<MessageBufferHandle_t>(params);

  // Allocate a buffer to hold the incoming command
  // The buffer size should be large enough to hold the largest command packet
  char cmdBuffer[MAX_CMD_SIZE]{0};

  DEBUG_ENTER("taskMoveServo()");

  FOREVER
  {
    DEBUG_HEARTBEAT("MoveServo");

    // Wait for a command to be received
    size_t bytesRead = xMessageBufferReceive(msgBufferHandle, cmdBuffer, sizeof(cmdBuffer), portMAX_DELAY);
    if (bytesRead > 0)
    {
      // Process the command
      int cmdType = cmdBuffer[0]; // First byte indicates the command type
      DEBUG_PRINTLN("Received command type: " + String(cmdType));
      DEBUG_PRINTLN("Command data:");
      String s = "";
      for (size_t i = 1; i < bytesRead; ++i)
      {
        s += String(cmdBuffer[i]);
      }
      DEBUG_PRINTLN("" + s);
    }
  }

  DEBUG_EXIT("taskMoveServo()");
}