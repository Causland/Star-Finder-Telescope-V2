#include "PlanTrajectoryCmd.h"

PlanTrajectoryCmd::PlanTrajectoryCmd() : Command(CommandID::CMD_PLAN_TRAJECTORY)
{

}

bool PlanTrajectoryCmd::serializeCommand()
{
  // Serialize the command ID
  if (!serialize(static_cast<uint8_t>(id))) return false;

  // Serialize the trajectory header
  if (!serialize(header)) return false;

  // Serialize the trajectory entries
  for (std::size_t i = 0; i < header.numEntries; ++i)
  {
    if (!serialize(entries[i])) return false;
  }

  return true;
}

bool PlanTrajectoryCmd::deserializeCommand()
{
  // Deserialize the command ID
  if (!deserialize(static_cast<uint8_t>(id))) return false;

  // Deserialize the trajectory header
  if (!deserialize(header)) return false;

  // Deserialize the trajectory entries
  for (std::size_t i = 0; i < header.numEntries; ++i)
  {
    if (!deserialize(entries[i])) return false;
  }

  return true;
}