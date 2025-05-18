#include <ArduCAM.h>
#include <SPI.h>
#include <Wire.h>

#include "Commands.h"
#include "Tasks.h"
#include "WiFiWrapper.h"
#include "Utils.h"

ArduCAM cam{OV2640, SS};
uint8_t camBuf[484];
ControlCameraCmd_t controlCameraCmd;

void taskControlCamera(void* params)
{
  DEBUG_ENTER("taskControlCamera()");

  ControlCameraParams* cameraParams = static_cast<ControlCameraParams*>(params);
  WiFiUDP* cameraSender = cameraParams->cameraSender;
  MessageBufferHandle_t msgBufferHandle = cameraParams->msgBufferHandle;
  Telemetry* telemetry = cameraParams->telemetry;
  EventGroupHandle_t startEvent = cameraParams->startEvent;

  Wire.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  SPI.begin();

  // Reset the CPLD
  cam.write_reg(0x07, 0x80);
  vTaskDelay(100 * portTICK_PERIOD_MS);
  cam.write_reg(0x07, 0x00);
  vTaskDelay(100 * portTICK_PERIOD_MS);

  while (true)
  {
    // Check if the SPI bus for the camera is ready with test
    cam.write_reg(ARDUCHIP_TEST1, 0x55);
    if (cam.read_reg(ARDUCHIP_TEST1) != 0x55)
    {
      DEBUG_CAMERA_PRINT("Waiting for camera SPI bus OK");
      vTaskDelay(1000 * portTICK_PERIOD_MS);
    }
    else
    {
      break;
    }

  }

  // Verify that the correct camera is connected
  // Should be OV2640_MINI_2MP
  while (true)
  {
    uint8_t vid{0};
    uint8_t pid{0};
    cam.wrSensorReg8_8(0xff, 0x01);
    cam.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    cam.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
    {
      DEBUG_CAMERA_PRINT("Waiting to find OV2640 module on SPI");
      vTaskDelay(1000 * portTICK_PERIOD_MS);
    }
    else
    {
      break;
    }
  }

  // Configure the camera with basic default settings
  cam.set_format(JPEG);
  cam.OV2640_set_JPEG_size(OV2640_640x480);
  cam.OV2640_set_Light_Mode(Auto);
  cam.OV2640_set_Color_Saturation(Saturation0);
  cam.OV2640_set_Brightness(Brightness0);
  cam.OV2640_set_Contrast(Contrast0);
  cam.OV2640_set_Special_effects(Normal);
  cam.InitCAM();
  vTaskDelay(1000 * portTICK_PERIOD_MS);

  cam.clear_fifo_flag();

  xEventGroupWaitBits(startEvent, BIT0, pdFALSE, pdTRUE, portMAX_DELAY);

  FOREVER
  {
    DEBUG_HEARTBEAT("ControlCamera");

    // Wait for a new camera command
    const size_t bytesRead = xMessageBufferReceive(msgBufferHandle, reinterpret_cast<uint8_t*>(&controlCameraCmd),
                                                   sizeof(ControlCameraCmd_t), portMAX_DELAY);
    if (bytesRead == sizeof(ControlCameraCmd_t))
    {
      switch(controlCameraCmd.id)
      {
        case CTRL_CAM_CONFIG:
        {
          DEBUG_CAMERA_PRINT("Received Camera Config Cmd: " +
                        String(controlCameraCmd.cfg.configID) + ", " +
                        String(controlCameraCmd.cfg.value));

          break;
        }
        case CTRL_CAM_PHOTO:
        {
          DEBUG_CAMERA_PRINT("Received Camera Photo Cmd");
          cam.flush_fifo();
          cam.clear_fifo_flag();

          DEBUG_CAMERA_PRINT("Starting Capture");
          cam.start_capture();

          // Wait for the capture to complete. Hopefully very quick
          // because not great for FreeRTOS
          while (!cam.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) { (void)0; }

          const uint32_t len = cam.read_fifo_length();
          if (len >= MAX_FIFO_SIZE)
          {
            DEBUG_CAMERA_PRINT("Camera FIFO over size");
            cam.clear_fifo_flag();
            break;
          }
          if (len == 0)
          {
            DEBUG_CAMERA_PRINT("Camera FIFO empty");
            cam.clear_fifo_flag();
            break;
          }

          DEBUG_CAMERA_PRINT("Receiving/Sending data via SPI");
          cam.CS_LOW();
          cam.set_fifo_burst();
          for (size_t i=0; i<len; ++i)
          {
            const size_t idx = i % sizeof(camBuf);
            if (i != 0 && idx == 0)
            {
              // Send the camera data
              cameraSender->beginPacket(WIFI_USER_ADDR, WIFI_CAMERA_PORT);
              cameraSender->write(camBuf, sizeof(camBuf));
              cameraSender->endPacket();
            }

            camBuf[idx] = SPI.transfer(0x00); // Transfer dummy byte to pump RX
          }
          // Send remaining buffer
          {
            // Send the camera data
            cameraSender->beginPacket(WIFI_USER_ADDR, WIFI_CAMERA_PORT);
            cameraSender->write(camBuf, len % sizeof(camBuf));
            cameraSender->endPacket();
          }
          
          // Finish transfer
          cam.CS_HIGH();
          cam.clear_fifo_flag();
          DEBUG_CAMERA_PRINT("Capture complete");
          break;
        }
        case CTRL_CAM_VIDEO:
        {
          DEBUG_CAMERA_PRINT("Received Camera Video Cmd: " +
                        String(controlCameraCmd.vid.duration));

          break;
        }
      }
    }
  }
}