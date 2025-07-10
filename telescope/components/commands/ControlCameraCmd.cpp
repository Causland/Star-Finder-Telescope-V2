#include "ControlCameraCmd.h"
#include "Serialization.h"

ControlCameraCmd::ControlCameraCmd() : Command(CommandID::CMD_CONTROL_CAMERA)
{

}

bool ControlCameraCmd::serializeCommand(uint8_t* buf, const std::size_t& size) const
{
  std::size_t bytesWritten{0};

  // Serialize the command ID
  auto result{Utils::serialize(buf, size, id)};
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the control camera command ID
  result = Utils::serialize(buf + bytesWritten, size - bytesWritten, ctrlCmdID);
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the config or video data
  switch (ctrlCmdID)
  {
    case CTRL_CAM_CONFIG:
      result = Utils::serialize(buf + bytesWritten, size - bytesWritten, cfg);
      if (result < 0) return false;
      break;
    case CTRL_CAM_VIDEO:
      result = Utils::serialize(buf + bytesWritten, size - bytesWritten, vid);
      if (result < 0) return false;
      break;
    default:
      // Unsupported command ID
      return false;
  }

  return true;
}

bool ControlCameraCmd::deserializeCommand(const uint8_t* buf, const std::size_t& size)
{
  std::size_t bytesRead{0};

  // Deserialize the command ID
  auto result{Utils::deserialize(buf, size, &id)};
  if (result < 0) return false;
  bytesRead += result;

  // Deserialize the control camera command ID
  result = Utils::deserialize(buf + bytesRead, size - bytesRead, &ctrlCmdID);
  if (result < 0) return false;
  bytesRead += result;

  // Deserialize the config or video data
  switch (ctrlCmdID)
  {
    case CTRL_CAM_CONFIG:
      result = Utils::deserialize(buf + bytesRead, size - bytesRead, &cfg);
      if (result < 0) return false;
      break;
    case CTRL_CAM_VIDEO:
      result = Utils::deserialize(buf + bytesRead, size - bytesRead, &vid);
      if (result < 0) return false;
      break;
    default:
      // Unsupported command ID
      return false;
  }

  return true;
}