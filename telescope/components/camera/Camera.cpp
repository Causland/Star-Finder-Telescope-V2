#include <chrono>
#include <esp_log.h>
#include <thread>
#include <Wire.h>

#include "Camera.h"

Camera::Camera()
{
  Wire.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  SPI.begin();
}

bool Camera::initialize()
{
  using std::chrono::milliseconds;

  // Reset the CPLD
  arduCam.write_reg(0x07, 0x80);
  std::this_thread::sleep_for(milliseconds(100));
  arduCam.write_reg(0x07, 0x00);
  std::this_thread::sleep_for(milliseconds(100));

  // Check if the SPI bus for the camera is ready with a test
  auto retries = 10;
  while (--retries)
  {
    arduCam.write_reg(ARDUCHIP_TEST1, 0x55);

    if (arduCam.read_reg(ARDUCHIP_TEST1) == 0x55) break;

    ESP_LOGD(TAG, "Waiting for camera SPI bus OK");
    std::this_thread::sleep_for(milliseconds(1000));
  }

  if (retries == 0)
  {
    ESP_LOGW(TAG, "Failed to receive SPI bus OK from camera");
    return false;
  }

  // Verify that the expected camera is connected
  retries = 10;
  while (--retries)
  {
    uint8_t vid{};
    uint8_t pid{};

    arduCam.wrSensorReg8_8(0xff, 0x01);
    arduCam.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    arduCam.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);

    if ((vid == 0x26) || ((pid == 0x41) && (pid == 0x42))) break;
  
    ESP_LOGD(TAG, "Waiting to find OV2640 module on I2C");
    std::this_thread::sleep_for(milliseconds(1000));
  }

  if (retries == 0)
  {
    ESP_LOGW(TAG, "Failed to find camera on I2C");
    return false;
  }

  configCam(JPEG, OV2640_640x480);

  arduCam.InitCAM();
  std::this_thread::sleep_for(milliseconds(1000));

  arduCam.clear_fifo_flag();

  return true;
}

void Camera::configCam(const uint8_t format, const uint8_t size)
{
  arduCam.set_format(format);
  arduCam.OV2640_set_JPEG_size(size);
}

void Camera::takePhoto()
{
  arduCam.flush_fifo();
  arduCam.clear_fifo_flag();

  ESP_LOGD(TAG, "Starting camera capture");
  arduCam.start_capture();

  while (!arduCam.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) { asm volatile("nop"); }

  const auto len{arduCam.read_fifo_length()};
  if (len >= MAX_FIFO_SIZE)
  {
    ESP_LOGE(TAG, "Camera FIFO over size");
    arduCam.clear_fifo_flag();
    return;
  }
  if (len == 0)
  {
    ESP_LOGE(TAG, "Camera FIFO empty");
    arduCam.clear_fifo_flag();
    return;
  }

  ESP_LOGD(TAG, "Receiving data via SPI and transmiting over Wifi");
  arduCam.CS_LOW();
  arduCam.set_fifo_burst();

  // Transfer the data from SPI into local buffer
  for (size_t i=0; i<len; ++i)
  {
    const size_t idx{i % camBuf.size()};
    if (i != 0 && idx == 0)
    {
      if (!cameraSender.send(camBuf.data(), camBuf.size()))
      {
        ESP_LOGE(TAG, "Failed to send camera data over Wifi");
      }
    }

    camBuf[idx] = SPI.transfer(0x00); // Send dummy byte to pump RX
  }

  // Send the remainder
  if (!cameraSender.send(camBuf.data(), len % sizeof(camBuf)))
  {
    ESP_LOGE(TAG, "Failed to send camera data over Wifi");
  }

  // Finish the transfer
  arduCam.CS_HIGH();
  arduCam.clear_fifo_flag();
  ESP_LOGD(TAG, "Capture complete!");
}

void Camera::takeTimelapse()
{

}

void Camera::takeVideo()
{

}