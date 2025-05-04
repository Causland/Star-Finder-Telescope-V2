#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <Arduino_FreeRTOS.h>
#include <lib/FreeRTOS-Kernel-v10.5.1\message_buffer.h>
#include <stdint.h>

#include "Tasks.h"

static constexpr uint16_t MAX_CMD_SIZE{128};

enum CommandID
{
  CMD_TELEM_RATE,
  CMD_PLAN_TRAJECTORY,
};

struct MoveServoCmd_t
{
  float az; ///< Target horizontal angle in degrees
  float el; ///< Target vertical angle in degrees
};

static constexpr uint8_t MAX_ENTRIES_PER_TRAJECTORY_PART{10}; ///< Max number of trajectory entries in a part
static constexpr uint8_t MAX_TRAJECTORY_PARTS{4}; ///< Max number of parts in a trajectory

struct TrajectoryHeader_t
{
  uint8_t seqNum; ///< Sequence number of part of the trajectory. Starts at 1
  uint8_t totalParts; ///< Total number of parts in the trajectory
  uint8_t numEntries; ///< Number of entries in this part of the trajectory
  uint8_t checksum; ///< Checksum of the trajectory. XOR of header and entries
};

struct TrajectoryEntry_t
{
  float t; ///< Time in seconds relative to the start of the trajectory
  float az; ///< Azimuth angle in degrees
  float el; ///< Elevation angle in degrees
};

struct TrajectoryPart_t
{
  TrajectoryHeader_t header;
  TrajectoryEntry_t entries[MAX_ENTRIES_PER_TRAJECTORY_PART]; ///< Array of trajectory entries
};

template <typename T>
static bool serialize(const T& obj, char* buffer, const size_t& bufferSize)
{
  if (bufferSize < sizeof(T))
  {
    return false;
  }
  memcpy(buffer, &obj, sizeof(T));
  return true;
}

template <typename T>
static bool deserialize(T* obj, const char* buffer, const size_t& bufferSize)
{
  if (bufferSize < sizeof(T))
  {
    return false;
  }
  memcpy(obj, buffer, sizeof(T));
  return true;
}

/// Create message buffers for each task to receive commands
///
/// @param[out] msgBufferHandles An array of message buffer handles that will be created for each task
///
/// @return true if all message buffers were allocated successfully
bool createCmdBuffers(MessageBufferHandle_t msgBufferHandles[NUM_TASKS]);

#endif