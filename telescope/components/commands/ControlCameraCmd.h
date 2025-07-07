#ifndef __CONTROL_CAMERA_CMD_H__
#define __CONTROL_CAMERA_CMD_H__

#include "Command.h"

class ControlCameraCmd : public Command
{
public:
  enum ControlCameraCmdID
  {
    CTRL_CAM_CONFIG,
    CTRL_CAM_PHOTO,
    CTRL_CAM_VIDEO,
  };

  ControlCameraCmd();

  struct ControlCameraConfigs_t
  {
    uint8_t configID; ///< Config item in ArduCAM.h
    uint8_t value; ///< New value for config item found in ArduCAM.h
  };

  struct ControlCameraVideo_t
  {
    uint32_t duration; ///< Duration of the video in seconds
  };

  bool serializeCommand() override;
  bool deserializeCommand() override;

  ControlCameraCmdID ctrlCmdID; ///< Specifies the underlying action to perform
  union
  {
    ControlCameraConfigs_t cfg;
    ControlCameraVideo_t vid;
  };
};

#endif // __CONTROL_CAMERA_CMD_H__