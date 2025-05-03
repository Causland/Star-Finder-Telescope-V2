#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Commands.h"
#include "Tasks.h"
#include "Utils.h"
#include "Wifi.h"

static constexpr uint16_t RECEIVE_PERIOD_MS{50};

void taskReceiveCommand(void* params)
{
  RecvCmdParams* cmdParams = static_cast<RecvCmdParams*>(params);
  WiFiUDP* cmdReceiver = static_cast<WiFiUDP*>(cmdParams->cmdReceiver);
  TaskHandle_t* taskHandles = static_cast<TaskHandle_t*>(cmdParams->taskHandles);
  MessageBufferHandle_t* msgBufferHandles = static_cast<MessageBufferHandle_t*>(cmdParams->msgBufferHandles);

  // Allocate a buffer to hold the incoming command
  // The buffer size should be large enough to hold the largest command packet
  char cmdBuffer[MAX_CMD_SIZE]{0};

  DEBUG_ENTER("taskReceiveCommand()");

  FOREVER
  {
    DEBUG_HEARTBEAT("ReceiveCommand");

    // Check if there are any incoming command packets
    int packetSize = cmdReceiver->parsePacket();
    if (packetSize > 0)
    {
      // Read first character from the packet to determine the command type
      int cmdType = cmdReceiver->peek();
      DEBUG_PRINTLN("Received command type: " + String(cmdType));
      
      // Select the command buffer based on the command type
      MessageBufferHandle_t cmdBufferHandle{nullptr};
      switch(cmdType)
      {
        case CMD_MOVE_SERVO:
          DEBUG_PRINTLN("Move Servo command received!");
          cmdBufferHandle = msgBufferHandles[TASK_MOVE_SERVO];
          break;
        case CMD_TELEM_RATE:
          DEBUG_PRINTLN("Telemetry rate command received!");
          cmdBufferHandle = msgBufferHandles[TASK_COLLECT_TELEMETRY];
          break;
        case CMD_PLAN_TRAJECTORY:
          DEBUG_PRINTLN("Plan trajectory command received!");
          cmdBufferHandle = msgBufferHandles[TASK_PLAN_TRAJECTORY];
          break;
      }
      if (cmdBufferHandle != nullptr)
      {
        // Read the command from the packet and send it to the appropriate task
        int bytesRead = cmdReceiver->read(cmdBuffer, sizeof(cmdBuffer));
        if (bytesRead > 0)
        {
          // Send the command to the task's message buffer
          if (xMessageBufferSend(cmdBufferHandle, cmdBuffer, bytesRead, 0) != bytesRead)
          {
            DEBUG_PRINTLN("Failed to send command to task message buffer!");
          }
        }
      }
      else
      {
        DEBUG_PRINTLN("Unknown command type received! " + String(cmdType));
      }
    }

    vTaskDelay(RECEIVE_PERIOD_MS * portTICK_PERIOD_MS);
  }

  DEBUG_EXIT("taskReceiveCommand()");
}