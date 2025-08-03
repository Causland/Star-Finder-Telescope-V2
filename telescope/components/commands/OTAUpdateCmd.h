#ifndef __OTA_UPDATE_CMD_H__
#define __OTA_UPDATE_CMD_H__

#include <array>
#include <cstdint>
#include <cstring>
#include <string>

#include "Command.h"

/// Command to perform an OTA update, allowing for the specification of
/// the update file name that will be used for the update.
class OTAUpdateCmd : public Command
{
public:
  /// Constructs an OTAUpdateCmd object with the OTA update command ID.
  OTAUpdateCmd();

  OTAUpdateCmd(const OTAUpdateCmd&) = default;
  OTAUpdateCmd(OTAUpdateCmd&&) = default;

  ~OTAUpdateCmd() override = default;

  OTAUpdateCmd& operator=(const OTAUpdateCmd&) = default;
  OTAUpdateCmd& operator=(OTAUpdateCmd&&) = default;

  /// Sets the name of the update file.
  ///
  /// @param[in] name the name of the update file
  ///
  /// @return true if the name fits within the name buffer, false otherwise
  bool setFilename(const std::string& name)
  {
    if (name.size() > filename.size() - 1) return false;
    filenameLen = name.size() + 1; // +1 for null terminator
    std::strncpy(filename.data(), name.c_str(), filename.size() - 1);
    return true;
  }

  /// @return the name of the update file
  std::string getFilename() const
  {
    return filename.data();
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
  std::array<char, UINT8_MAX> filename{}; ///< The name of the file
};

#endif // __OTA_UPDATE_CMD_H__