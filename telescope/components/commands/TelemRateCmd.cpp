#include "TelemRateCmd.h"

TelemRateCmd::TelemRateCmd() : Command(CommandID::CMD_TELEM_RATE)
{

}

TelemRateCmd::TelemRateCmd(const uint16_t rate) 
                            : Command(CommandID::CMD_TELEM_RATE), rate(rate)
{

}

bool TelemRateCmd::serializeCommand()
{
  // Serialize the command ID
  if (!serialize(static_cast<uint8_t>(id))) return false;

  // Serialize the telemetry rate
  if (!serialize(rate)) return false;

  return true;
}

bool TelemRateCmd::deserializeCommand()
{
  // Deserialize the command ID
  if (!deserialize(static_cast<uint8_t>(id))) return false;

  // Deserialize the telemetry rate
  if (!deserialize(rate)) return false;

  return true;
}