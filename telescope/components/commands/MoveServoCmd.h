#ifndef __MOVE_SERVO_CMD_H__
#define __MOVE_SERVO_CMD_H__

#include "Command.h"

class MoveServoCmd : public Command
{
public:
  MoveServoCmd();
  MoveServoCmd(const float& az, const float& el);

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

  float az; ///< Target horizontal angle in degrees
  float el; ///< Target vertical angle in degrees
};

#endif // __MOVE_SERVO_CMD_H__