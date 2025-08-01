#ifndef __OTA_UPDATE_CMD_H__
#define __OTA_UPDATE_CMD_H__

#include <cstdint>
#include <cstring>
#include <string>

#include "Command.h"

class OTAUpdateCmd : public Command
{
public:
  OTAUpdateCmd();

  bool setFilename(const std::string& name)
  {
    if (name.size() > sizeof(filename) - 1) return false;
    filenameLen = name.size() + 1; // +1 for null terminator
    std::strncpy(filename, name.c_str(), sizeof(filename) - 1);
    return true;
  }

  std::string getFilename() const
  {
    return filename;
  }

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

  uint8_t filenameLen{}; ///< The update file name length including null terminator
  char filename[UINT8_MAX]{}; ///< The name of the file
  
};

#endif // __OTA_UPDATE_CMD_H__