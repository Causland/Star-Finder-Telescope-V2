#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <ArduCAM.h>
#include <SPI.h>

#include "NetworkConstants.h"
#include "UDPSender.h"

class Camera
{
public:
  Camera();

  bool initialize();

  void configCam(const uint8_t format, const uint8_t size);

  void takePhoto();
  void takeTimelapse();
  void takeVideo();

private:
  static constexpr std::size_t CAMERA_BUFFER_SIZE{508};
  static constexpr const char* TAG{"CAM"};

  bool sendCamData();

  ArduCAM arduCam{OV2640, SS};
  UDPSender cameraSender{"CamSender", WIFI_USER_ADDR, WIFI_CAMERA_PORT};
  std::array<uint8_t, CAMERA_BUFFER_SIZE> camBuf{};
};

#endif