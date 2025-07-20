#include "Serialization.h"
#include "FocusCmd.h"

FocusCmd::FocusCmd() : Command(CommandID::CMD_FOCUS)
{

}

bool FocusCmd::serializeCommand(uint8_t* buf, const std::size_t& size) const
{
  std::size_t bytesWritten{0};

  // Serialize the command ID
  auto result{Utils::serialize(buf, size, id)};
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the focus command ID
  result = Utils::serialize(buf + bytesWritten, size - bytesWritten, focusCmdID);
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the manual focus degrees if needed
  switch(focusCmdID)
  {
    case FOCUS_MANUAL:
      result = Utils::serialize(buf + bytesWritten, size - bytesWritten, man.degrees);
      if (result < 0) return false;
      break;
    case FOCUS_AUTO:
      break;
    default:
      // Unsupported command ID
      return false;
  }

  return true;
}

bool FocusCmd::deserializeCommand(const uint8_t* buf, const std::size_t& size)
{
  std::size_t bytesRead{0};

  // Deserialize the command ID
  auto result{Utils::deserialize(buf, size, &id)};
  if (result < 0) return false;
  bytesRead += result;

  // Deerialize the focus command ID
  result = Utils::deserialize(buf + bytesRead, size - bytesRead, &focusCmdID);
  if (result < 0) return false;
  bytesRead += result;

  // Serialize the manual focus degrees if needed
  switch(focusCmdID)
  {
    case FOCUS_MANUAL:
      result = Utils::deserialize(buf + bytesRead, size - bytesRead, &man.degrees);
      if (result < 0) return false;
      break;
    case FOCUS_AUTO:
      break;
    default:
      // Unsupported command ID
      return false;
  }

  return true;
}