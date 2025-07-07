#ifndef __MOVE_SERVO_CMD_H__
#define __MOVE_SERVO_CMD_H__

#include "Command.h"

class MoveServoCmd : public Command
{
public:
  MoveServoCmd();
  MoveServoCmd(const float& az, const float& el);

  bool serializeCommand() override;
  bool deserializeCommand() override;

  float az; ///< Target horizontal angle in degrees
  float el; ///< Target vertical angle in degrees
};

#endif // __MOVE_SERVO_CMD_H__