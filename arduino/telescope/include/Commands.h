#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <freertos/FreeRTOS.h>
#include <freertos/message_buffer.h>
#include <stdint.h>

#include "Tasks.h"

static constexpr uint16_t MAX_CMD_SIZE{128};

enum CommandID
{
  CMD_TELEM_RATE,
  CMD_PLAN_TRAJECTORY,
  CMD_CONTROL_CAMERA,
};

struct TelemetryRateCmd_t
{
  uint16_t rate; ///< Telemetry rate in milliseconds
};
static constexpr uint8_t MIN_TELEM_RATE_MS{100}; ///< Min telemetry rate in milliseconds

struct MoveServoCmd_t
{
  float az; ///< Target horizontal angle in degrees
  float el; ///< Target vertical angle in degrees
};

enum ControlCameraCmdID
{
  CTRL_CAM_CONFIG,
  CTRL_CAM_PHOTO,
  CTRL_CAM_VIDEO,
};

struct ControlCameraConfigs_t
{
  uint8_t configID; ///< Config item in ArduCAM.h
  uint8_t value; ///< New value for config item found in ArduCAM.h
};

struct ControlCameraVideo_t
{
  uint32_t duration; ///< Duration of the video in seconds
};

struct ControlCameraCmd_t
{
  ControlCameraCmdID id; ///< Specifies the underlying action to perform
  union
  {
    ControlCameraConfigs_t cfg;
    ControlCameraVideo_t vid;
  };
};

static constexpr uint8_t MAX_ENTRIES_PER_TRAJECTORY_PART{10}; ///< Max number of trajectory entries in a part
static constexpr uint8_t MAX_TRAJECTORY_PARTS{4}; ///< Max number of parts in a trajectory

struct TrajectoryHeader_t
{
  uint8_t seqNum; ///< Sequence number of part of the trajectory. Starts at 1
  uint8_t totalParts; ///< Total number of parts in the trajectory
  uint8_t numEntries; ///< Number of entries in this part of the trajectory
};

struct TrajectoryEntry_t
{
  float t; ///< Time in seconds relative to the start of the trajectory
  float az; ///< Azimuth angle in degrees
  float el; ///< Elevation angle in degrees
};

#pragma pack(push, 1) // Ensure no padding between struct members
struct TrajectoryPart_t
{
  TrajectoryHeader_t header;
  TrajectoryEntry_t entries[MAX_ENTRIES_PER_TRAJECTORY_PART]; ///< Array of trajectory entries
};
#pragma pack(pop) // Restore previous packing alignment

template <typename T>
static bool serialize(const T& obj, uint8_t* buffer, const size_t& bufferSize)
{
  if (bufferSize < sizeof(T))
  {
    return false;
  }

  // Convert to network byte order
  const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&obj);
  for (size_t i = 0; i < sizeof(T); ++i)
  {
    buffer[i] = ptr[sizeof(T) - 1 - i];
  }
  return true;
}

template <typename T>
static bool deserialize(T* obj, const char* buffer, const size_t& bufferSize)
{
  if (bufferSize < sizeof(T))
  {
    return false;
  }

  // Convert from network byte order
  const uint8_t* ptr = reinterpret_cast<const uint8_t*>(buffer);
  for (size_t i = 0; i < sizeof(T); ++i)
  {
    ((uint8_t*)obj)[sizeof(T) - 1 - i] = ptr[i];
  }
  return true;
}

/// Create message buffers for each task to receive commands
///
/// @param[out] msgBufferHandles An array of message buffer handles that will be created for each task
///
/// @return true if all message buffers were allocated successfully
bool createCmdBuffers(MessageBufferHandle_t msgBufferHandles[NUM_TASKS]);

#endif