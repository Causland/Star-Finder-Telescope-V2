#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

#include "Commands.h"
#include "Tasks.h"
#include "Telemetry.h"
#include "Utils.h"

// Statically allocate a buffer to hold the telemetry data
char telemBuffer[sizeof(Telemetry) * 2]{0};
uint16_t telemRate{250}; ///< Telemetry rate in milliseconds

void taskCollectTelemetry(void* params)
{
  DEBUG_ENTER("taskCollectTelemetry()");

  CollectTelemetryParams* telemParams = static_cast<CollectTelemetryParams*>(params);
  WiFiUDP* telemSender = telemParams->telemSender;
  MessageBufferHandle_t msgBufferHandle = telemParams->msgBufferHandle;
  Telemetry* telemetry = telemParams->telemetry;

  vTaskDelay(3000 * portTICK_PERIOD_MS); // Wait 3 seconds for system to start up

  FOREVER
  {
    DEBUG_HEARTBEAT("CollectTelemetry");

    // Check if there is a new telemetry rate command in the message buffer
    size_t bytesRead = xMessageBufferReceive(msgBufferHandle, telemBuffer, sizeof(telemBuffer), 0);
    if (bytesRead > 0)
    {
      // Deserialize the telemetry rate command
      TelemetryRateCmd_t* cmd = reinterpret_cast<TelemetryRateCmd_t*>(telemBuffer);
      DEBUG_TELEMETRY("Telemetry rate command received: " + String(cmd->rate) + " ms");

      // Check if the command is valid
      if (cmd->rate < MIN_TELEM_RATE_MS)
      {
        DEBUG_PRINTLN("ERROR - Telemetry rate command is invalid: " + String(cmd->rate) + " ms"
                      ". Setting to minimum: " + String(MIN_TELEM_RATE_MS) + " ms");
        cmd->rate = MIN_TELEM_RATE_MS;
      }

      telemRate = cmd->rate;
    }

    // Report the telemetry data
    DEBUG_TELEMETRY("Collecting telemetry data...");
    int bytesSerialized = telemetry->serializeTelemetry(telemBuffer, sizeof(telemBuffer));
    if (bytesSerialized > 0)
    {
      // Send the telemetry data to the UDP sender
      telemSender->beginPacket(WIFI_USER_ADDR, WIFI_TELEM_PORT);
      telemSender->write(telemBuffer, bytesSerialized);
      telemSender->endPacket();
    }
    else
    {
      DEBUG_PRINTLN("ERROR - Failed to serialize telemetry data!");
    }

    vTaskDelay(telemRate * portTICK_PERIOD_MS);
  }

  DEBUG_EXIT("taskCollectTelemetry()");
}