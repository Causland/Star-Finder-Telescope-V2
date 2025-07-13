#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <cstdint>

/// The command base class for all commands containing
/// the command header and any supporting functions.
class Command
{
public:
  enum CommandID : uint8_t
  {
    CMD_TELEM_RATE,
    CMD_PLAN_TRAJECTORY,
    CMD_MOVE_BASE_SERVOS,
    CMD_CONTROL_CAMERA,
    NUM_COMMANDS,
  };

  static constexpr std::size_t MAX_CMD_SIZE{128};
  static constexpr std::size_t POS_CMD_ID{0};

  explicit Command(const CommandID& cmdID) : id(cmdID) {}

  /// Serialize the command data into the provided buffer.
  ///
  /// @param[in] buf The buffer to serialize into
  /// @param[in] size The size of the buffer
  ///
  /// @return true if serialization was successful
  virtual bool serializeCommand(uint8_t* buf, const std::size_t& size) const = 0;

  /// Deserialize the command data from the buffer.
  ///
  /// @param[in] buf The buffer to deserialize from
  /// @param[in] size The size of the buffer
  ///
  /// @return true if deserialization was successful
  virtual bool deserializeCommand(const uint8_t* buf, const std::size_t& size) = 0;

  CommandID id; ///< The ID of the command
};

#endif