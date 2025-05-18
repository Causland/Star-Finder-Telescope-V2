#include "Commands.h"
#include "Tasks.h"
#include "Utils.h"
#include "WiFiWrapper.h"

static constexpr uint16_t RECEIVE_PERIOD_MS{50};

void taskReceiveCommand(void* params)
{
  DEBUG_ENTER("taskReceiveCommand()");

  RecvCmdParams* cmdParams = static_cast<RecvCmdParams*>(params);
  WiFiUDP* cmdReceiver = cmdParams->cmdReceiver;
  TaskHandle_t* taskHandles = cmdParams->taskHandles;
  MessageBufferHandle_t* msgBufferHandles = cmdParams->msgBufferHandles;
  Telemetry* telemetry = cmdParams->telemetry;
  EventGroupHandle_t startEvent = cmdParams->startEvent;

  // Register task telemetry
  uint16_t cmdsReceived{0};
  telemetry->registerTelemFieldCmdsReceived(&cmdsReceived);

  // Allocate a buffer to hold the incoming command
  // The buffer size should be large enough to hold the largest command packet
  char cmdBuffer[MAX_CMD_SIZE]{0};

  xEventGroupWaitBits(startEvent, BIT0, pdFALSE, pdTRUE, portMAX_DELAY);

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
        case CMD_TELEM_RATE:
          DEBUG_PRINTLN("Telemetry rate command received!");
          cmdBufferHandle = msgBufferHandles[TASK_COLLECT_TELEMETRY];
          break;
        case CMD_PLAN_TRAJECTORY:
          DEBUG_PRINTLN("Plan trajectory command received!");
          cmdBufferHandle = msgBufferHandles[TASK_PLAN_TRAJECTORY];
          break;
        case CMD_CONTROL_CAMERA:
          DEBUG_PRINTLN("Control camera command received!");
          cmdBufferHandle = msgBufferHandles[TASK_CONTROL_CAMERA];
      }
      if (cmdBufferHandle != nullptr)
      {
        // Read the command from the packet and send it to the appropriate task
        int bytesRead = cmdReceiver->read(cmdBuffer, sizeof(cmdBuffer));
        if (bytesRead > 0)
        {
          ++cmdsReceived;
          
          // Send the command to the task's message buffer. Strip the command type
          if (xMessageBufferSend(cmdBufferHandle, cmdBuffer + 1, bytesRead - 1, 0) != bytesRead - 1)
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