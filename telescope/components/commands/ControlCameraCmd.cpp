#include "ControlCameraCmd.h"

ControlCameraCmd::ControlCameraCmd() : Command(CommandID::CMD_CONTROL_CAMERA)
{

}

bool ControlCameraCmd::serializeCommand()
{
  // Serialize the command ID
  if (!serialize(static_cast<uint8_t>(id))) return false;

  // Serialize the control camera command ID
  if (!serialize(ctrlCmdID)) return false;

  // Serialize the config or video data
  switch (ctrlCmdID)
  {
    case CTRL_CAM_CONFIG:
      if (!serialize(cfg)) return false;
      break;
    case CTRL_CAM_VIDEO:
      if (!serialize(vid)) return false;
      break;
    default:
      // Unsupported command ID
      return false;
  }

  return true;
}

bool ControlCameraCmd::deserializeCommand()
{
  // Deserialize the command ID
  if (!deserialize(static_cast<uint8_t>(id))) return false;

  // Deserialize the control camera command ID
  if (!deserialize(ctrlCmdID)) return false;

  // Deserialize the config or video data
  switch (ctrlCmdID)
  {
    case CTRL_CAM_CONFIG:
      if (!deserialize(cfg)) return false;
      break;
    case CTRL_CAM_VIDEO:
      if (!deserialize(vid)) return false;
      break;
    default:
      // Unsupported command ID
      return false;
  }

  return true;
}