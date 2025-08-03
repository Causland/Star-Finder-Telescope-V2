#ifndef __CONTROL_CAMERA_CMD_H__
#define __CONTROL_CAMERA_CMD_H__

#include "Command.h"

/// Command to control the camera module, allowing configuration,
/// photo capture, timelapse, and video recording.
class ControlCameraCmd : public Command
{
public:
  enum ControlCameraCmdID : uint8_t
  {
    CTRL_CAM_CONFIG,
    CTRL_CAM_PHOTO,
    CTRL_CAM_VIDEO,
  };

  /// Constructs a ControlCameraCmd object with the control camera command ID.
  ControlCameraCmd();

  ControlCameraCmd(const ControlCameraCmd&) = default;
  ControlCameraCmd(ControlCameraCmd&&) = default;

  ~ControlCameraCmd() override = default;

  ControlCameraCmd& operator=(const ControlCameraCmd&) = default;
  ControlCameraCmd& operator=(ControlCameraCmd&&) = default;

  struct ControlCameraConfigs_t
  {
    uint8_t configID; ///< Config item in ArduCAM.h
    uint8_t value; ///< New value for config item found in ArduCAM.h
  };

  struct ControlCameraVideo_t
  {
    uint32_t duration; ///< Duration of the video in seconds
  };

  /// Serialize the command data into the provided buffer.
  ///
  /// @param[in] buf The buffer to serialize into
  /// @param[in] size The size of the buffer
  ///
  /// @return true if serialization was successful
  virtual bool serializeCommand(uint8_t* buf, const std::size_t& size) const override;

  /// Deserialize the command data from the buffer.
  ///
  /// @param[in] buf The buffer to deserialize from
  /// @param[in] size The size of the buffer
  ///
  /// @return true if deserialization was successful
  virtual bool deserializeCommand(const uint8_t* buf, const std::size_t& size) override;

  ControlCameraCmdID ctrlCmdID; ///< Specifies the underlying action to perform
  union
  {
    ControlCameraConfigs_t cfg; ///< Configuration settings for the camera.
    ControlCameraVideo_t vid; ///< Video parameters for recording.
  };
};

#endif // __CONTROL_CAMERA_CMD_H__