#include "Serialization.h"
#include "TelemRateCmd.h"

TelemRateCmd::TelemRateCmd() : Command(CommandID::CMD_TELEM_RATE)
{

}

TelemRateCmd::TelemRateCmd(const uint16_t rate) 
                            : Command(CommandID::CMD_TELEM_RATE), rate(rate)
{

}

bool TelemRateCmd::serializeCommand(uint8_t* buf, const std::size_t& size) const
{
  std::size_t bytesWritten{0};

  // Serialize the command ID
  auto result{Utils::serialize(buf, size, id)};
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the telemetry rate
  result = Utils::serialize(buf + bytesWritten,
                            size - bytesWritten,
                            rate);
  if (result < 0) return false;

  return true;
}

bool TelemRateCmd::deserializeCommand(const uint8_t* buf, const std::size_t& size)
{
  std::size_t bytesRead{0};

  // Deserialize the command ID
  auto result{Utils::deserialize(buf, size, &id)};
  if (result < 0) return false;
  bytesRead += result;

  // Deserialize the telemetry rate
  result = Utils::deserialize(buf + bytesRead, size - bytesRead, &rate);
  if (result < 0) return false;

  return true;
}