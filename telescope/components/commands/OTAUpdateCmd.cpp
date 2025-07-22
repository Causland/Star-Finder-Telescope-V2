#include <cstring>

#include "Serialization.h"
#include "OTAUpdateCmd.h"

OTAUpdateCmd::OTAUpdateCmd() : Command(CommandID::CMD_OTA_UPDATE)
{

}

bool OTAUpdateCmd::serializeCommand(uint8_t* buf, const std::size_t& size) const
{
  std::size_t bytesWritten{0};

  // Serialize the command ID
  auto result{Utils::serialize(buf, size, id)};
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the command fields
  result = Utils::serialize(buf + bytesWritten, size - bytesWritten, filenameLen);
  if (result < 0) return false;
  bytesWritten += result;

  if (size - bytesWritten < filenameLen) return false;
  std::strncpy(reinterpret_cast<char*>(buf + bytesWritten), filename, filenameLen);

  return true;
}

bool OTAUpdateCmd::deserializeCommand(const uint8_t* buf, const std::size_t& size)
{
  std::size_t bytesRead{0};

  // Deserialize the command ID
  auto result{Utils::deserialize(buf, size, &id)};
  if (result < 0) return false;
  bytesRead += result;

  // Deserialize the fields
  result = Utils::deserialize(buf + bytesRead, size - bytesRead, &filenameLen);
  if (result < 0) return false;
  bytesRead += result;
  
  if (size - bytesRead < filenameLen) return false;
  std::strncpy(filename, reinterpret_cast<const char*>(buf + bytesRead), filenameLen);

  return true;
}