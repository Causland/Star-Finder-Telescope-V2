#include "MoveServoCmd.h"

MoveServoCmd::MoveServoCmd() : Command(CommandID::CMD_MOVE_SERVO)
{

}

MoveServoCmd::MoveServoCmd(const float& az, const float& el)
                            : Command(CommandID::CMD_MOVE_SERVO), 
                              az(az), el(el)
{

}

bool MoveServoCmd::serializeCommand()
{
  // Serialize the command ID
  if (!serialize(static_cast<uint8_t>(id))) return false;

  // Serialize the az
  if (!serialize(az)) return false;

  // Serialize the el
  if (!serialize(el)) return false;

  return true;
}

bool MoveServoCmd::deserializeCommand()
{
  // Deserialize the command ID
  if (!deserialize(static_cast<uint8_t>(id))) return false;

  // Deserialize the az
  if (!deserialize(az)) return false;

  // Deserialize the el
  if (!deserialize(el)) return false;

  return true;
}