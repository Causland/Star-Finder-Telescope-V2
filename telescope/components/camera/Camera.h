#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <ArduCAM.h>
#include <SPI.h>

#include "NetworkConstants.h"
#include "UDPSender.h"

/// Provides an interface to the camera module, allowing for photo,
/// timelapse, and video capture.
class Camera
{
public:
  /// Constructs a Camera object, initializing the SPI bus and I2C for
  /// communication with the camera module.
  Camera();

  /// Initializes the camera module, checking the SPI bus and I2C connection.
  bool initialize();

  /// Configures the camera with the specified format and resolution size.
  ///
  /// @param[in] format the image format (e.g., JPEG)
  /// @param[in] size the resolution size (e.g., OV2640_640x480)
  void configCam(const uint8_t format, const uint8_t size);

  /// Takes a photo with the camera. Capture an image and send over WiFi.
  void takePhoto();

  /// Takes a timelapse with the camera at the specified interval and duration and
  /// sends the images over WiFi.
  ///
  /// @param[in] interval the time between captures in milliseconds.
  /// @param[in] duration the total duration of the timelapse in milliseconds.
  void takeTimelapse(const std::chrono::milliseconds& interval,
                     const std::chrono::milliseconds& duration);

  /// Takes a video with the camera at the specified fps and duration and sends each frame
  /// over WiFi.
  /// The camera will attempt to capture video frames at close to the specified fps.
  ///
  /// @param[in] fps the target frames per second for the video.
  /// @param[in] duration the total duration of the video in milliseconds.
  void takeVideo(const uint16_t fps,
                 const std::chrono::milliseconds& duration);

private:
  static constexpr std::size_t CAMERA_BUFFER_SIZE{508}; ///< Size of the camera buffer for sending data
  static constexpr const char* TAG{"CAM"}; ///< The tag for logging.

  /// Sends the camera data over Wifi.
  ///
  /// @return true if the data was sent successfully, false otherwise.
  bool sendCamData();

  ArduCAM arduCam{OV2640, SS}; ///< The ArduCAM object for camera operations.
  UDPSender cameraSender{"CamSender", WIFI_USER_ADDR, WIFI_CAMERA_PORT}; ///< The UDP Sender for camera data.
  std::array<uint8_t, CAMERA_BUFFER_SIZE> camBuf{}; ///< Buffer for camera data.
};

#endif