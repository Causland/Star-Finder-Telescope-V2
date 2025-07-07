#ifndef __PLAN_TRAJECTORY_CMD_H__
#define __PLAN_TRAJECTORY_CMD_H__

#include "Command.h"


class PlanTrajectoryCmd : public Command
{
public:
  static constexpr uint8_t MAX_ENTRIES_PER_TRAJECTORY_PART{10}; ///< Max number of trajectory entries in a part

  PlanTrajectoryCmd();

  #pragma pack(push, 1) // Ensure no padding between struct members
  struct TrajectoryHeader_t
  {
    uint8_t seqNum; ///< Sequence number of part of the trajectory. Starts at 1
    uint8_t totalParts; ///< Total number of parts in the trajectory
    uint8_t numEntries; ///< Number of entries in this part of the trajectory
  };
  #pragma pack(pop) // Restore previous packing alignment

  #pragma pack(push, 1) // Ensure no padding between struct members
  struct TrajectoryEntry_t
  {
    float t; ///< Time in seconds relative to the start of the trajectory
    float az; ///< Azimuth angle in degrees
    float el; ///< Elevation angle in degrees
  };
  #pragma pack(pop) // Restore previous packing alignment

  bool serializeCommand() override;
  bool deserializeCommand() override;

  TrajectoryHeader_t header; ///< Header for the trajectory part
  std::array<TrajectoryEntry_t, MAX_ENTRIES_PER_TRAJECTORY_PART> entries; ///< Entries in the trajectory part
};

#endif // __PLAN_TRAJECTORY_CMD_H__