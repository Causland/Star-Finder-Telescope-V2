#include "MoveServoCmd.h"
#include "Serialization.h"

MoveServoCmd::MoveServoCmd() : Command(CommandID::CMD_MOVE_SERVO)
{

}

MoveServoCmd::MoveServoCmd(const float& az, const float& el)
                            : Command(CommandID::CMD_MOVE_SERVO), 
                              az(az), el(el)
{

}

bool MoveServoCmd::serializeCommand(uint8_t* buf, const std::size_t& size) const
{
  std::size_t bytesWritten{0};

  // Serialize the command ID
  auto result{Utils::serialize(buf, size, id)};
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the az
  result = Utils::serialize(buf + bytesWritten, size - bytesWritten, az);
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the el
  result = Utils::serialize(buf + bytesWritten, size - bytesWritten, el);
  if (result < 0) return false;

  return true;
}

bool MoveServoCmd::deserializeCommand(const uint8_t* buf, const std::size_t& size)
{
  std::size_t bytesRead{0};

  // Deserialize the command ID
  auto result{Utils::deserialize(buf, size, &id)};
  if (result < 0) return false;
  bytesRead += result;

  // Deserialize the az
  result = Utils::deserialize(buf + bytesRead, size - bytesRead, &az);
  if (result < 0) return false;
  bytesRead += result;

  // Deserialize the el
  result = Utils::deserialize(buf + bytesRead, size - bytesRead, &el);
  if (result < 0) return false;

  return true;
}