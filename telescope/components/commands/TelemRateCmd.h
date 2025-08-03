#ifndef __TELEM_RATE_CMD_H__
#define __TELEM_RATE_CMD_H__

#include <cstdint>

#include "Command.h"

/// Command to set the telemetry rate for the telescope system.
class TelemRateCmd : public Command
{
public:
  /// Constructs a TelemRateCmd object with the telemetry rate command ID.
  TelemRateCmd();

  /// Constructs a TelemRateCmd object with the specified telemetry rate.
  ///
  /// @param[in] rate the telemetry rate in milliseconds
  explicit TelemRateCmd(const uint16_t rate);

  TelemRateCmd(const TelemRateCmd&) = default;
  TelemRateCmd(TelemRateCmd&&) = default;

  ~TelemRateCmd() override = default;

  TelemRateCmd& operator=(const TelemRateCmd&) = default;
  TelemRateCmd& operator=(TelemRateCmd&&) = default;

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

  uint16_t rate{}; ///< Telemetry rate in milliseconds
};

#endif // __TELEM_RATE_CMD_H__