#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <Arduino_FreeRTOS.h>
#include <lib/FreeRTOS-Kernel-v10.5.1\message_buffer.h>
#include <stdint.h>

#include "Tasks.h"

static constexpr uint16_t MAX_CMD_SIZE{100};

enum CommandID
{
  CMD_MOVE_SERVO,
  CMD_TELEM_RATE,
  CMD_PLAN_TRAJECTORY,
};

struct MoveServoCmd_t
{
  float vertAngle; ///< Target vertical angle in degrees
  float horizAngle; ///< Target horizontal angle in degrees
};

struct MoveServoCmd
{
  static bool serialize(const MoveServoCmd_t& cmd, char* buffer, const size_t& bufferSize)
  {
    if (bufferSize < sizeof(MoveServoCmd_t))
    {
      return false;
    }
    memcpy(buffer, &cmd, sizeof(MoveServoCmd_t));
    return true;
  }

  static bool deserialize(MoveServoCmd_t* cmd, const char* buffer, const size_t& bufferSize)
  {
    if (bufferSize < sizeof(MoveServoCmd_t))
    {
      return false;
    }
    memcpy(&cmd, buffer, sizeof(MoveServoCmd));
  }
};

/// Create message buffers for each task to receive commands
///
/// @param[out] msgBufferHandles An array of message buffer handles that will be created for each task
///
/// @return true if all message buffers were allocated successfully
bool createCmdBuffers(MessageBufferHandle_t msgBufferHandles[NUM_TASKS]);

#endif