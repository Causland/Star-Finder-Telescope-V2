#ifndef __MOVE_BASE_SERVOS_CMD_H__
#define __MOVE_BASE_SERVOS_CMD_H__

#include "Command.h"

/// Command to move the base servos of the telescope by specifying
/// target angles for azimuth and elevation.
class MoveBaseServosCmd : public Command
{
public:
  /// Constructs a MoveBaseServosCmd object with move base servos command ID.
  MoveBaseServosCmd();

  /// Constructs a MoveBaseServosCmd object with specified azimuth and elevation angles.
  ///
  /// @param[in] az target horizontal angle in degrees
  /// @param[in] el target vertical angle in degrees
  MoveBaseServosCmd(const float& az, const float& el);

  MoveBaseServosCmd(const MoveBaseServosCmd&) = default;
  MoveBaseServosCmd(MoveBaseServosCmd&&) = default;

  ~MoveBaseServosCmd() override = default;

  MoveBaseServosCmd& operator=(const MoveBaseServosCmd&) = default;
  MoveBaseServosCmd& operator=(MoveBaseServosCmd&&) = default;

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

  float az{}; ///< Target horizontal angle in degrees
  float el{}; ///< Target vertical angle in degrees
};

#endif // __MOVE_BASE_SERVOS_CMD_H__