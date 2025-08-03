#ifndef __PLAN_TRAJECTORY_CMD_H__
#define __PLAN_TRAJECTORY_CMD_H__

#include <array>

#include "Command.h"

/// Command to plan a trajectory for the telescope. Contains multiple entries
/// that specify the azimuth and elevation angles at specific times.
class PlanTrajectoryCmd : public Command
{
public:
  static constexpr uint8_t MAX_ENTRIES_PER_TRAJECTORY_PART{10}; ///< Max number of trajectory entries in a part

  /// Constructs a PlanTrajectoryCmd object with the plan trajectory command ID.
  PlanTrajectoryCmd();

  PlanTrajectoryCmd(const PlanTrajectoryCmd&) = default;
  PlanTrajectoryCmd(PlanTrajectoryCmd&&) = default;

  ~PlanTrajectoryCmd() override = default;

  PlanTrajectoryCmd& operator=(const PlanTrajectoryCmd&) = default;
  PlanTrajectoryCmd& operator=(PlanTrajectoryCmd&&) = default;

  #pragma pack(push, 1) // Ensure no padding between struct members
  struct TrajectoryHeader_t
  {
    uint8_t seqNum{}; ///< Sequence number of part of the trajectory. Starts at 1
    uint8_t totalParts{}; ///< Total number of parts in the trajectory
    uint8_t numEntries{}; ///< Number of entries in this part of the trajectory
  };
  #pragma pack(pop) // Restore previous packing alignment

  #pragma pack(push, 1) // Ensure no padding between struct members
  struct TrajectoryEntry_t
  {
    float t{}; ///< Time in seconds relative to the start of the trajectory
    float az{}; ///< Azimuth angle in degrees
    float el{}; ///< Elevation angle in degrees
  };
  #pragma pack(pop) // Restore previous packing alignment

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

  TrajectoryHeader_t header{}; ///< Header for the trajectory part
  std::array<TrajectoryEntry_t, MAX_ENTRIES_PER_TRAJECTORY_PART> entries{}; ///< Entries in the trajectory part
};

#endif // __PLAN_TRAJECTORY_CMD_H__