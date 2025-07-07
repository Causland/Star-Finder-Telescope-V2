#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Serializer.h"

static constexpr std::size_t MAX_CMD_SIZE{128};

/// The command base class for all commands containing
/// the command header and any supporting functions.
class Command : public Serializer<MAX_CMD_SIZE>
{
public:
  enum CommandID : uint8_t
  {
    CMD_TELEM_RATE,
    CMD_PLAN_TRAJECTORY,
    CMD_MOVE_SERVO,
    CMD_CONTROL_CAMERA,
  };

  explicit Command(const CommandID& cmdID) : id(cmdID) {}

  /// Look into the command buffer and extract the command ID
  /// without deserializing the data.
  CommandID peekCmdID() const
  {
    return static_cast<CommandID>(getBuffer()[0]);
  }

  /// Serialize the command data into the buffer.
  ///
  /// @return true if serialization was successful
  virtual bool serializeCommand() = 0;

  /// Deserialize the command data from the buffer.
  ///
  /// @return true if deserialization was successful
  virtual bool deserializeCommand() = 0;

  CommandID id; ///< The ID of the command
};

#endif