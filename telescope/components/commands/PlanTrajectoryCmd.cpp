#include "PlanTrajectoryCmd.h"
#include "Serialization.h"

PlanTrajectoryCmd::PlanTrajectoryCmd() : Command(CommandID::CMD_PLAN_TRAJECTORY)
{

}

bool PlanTrajectoryCmd::serializeCommand(uint8_t* buf, const std::size_t& size) const
{
  std::size_t bytesWritten{0};

  // Serialize the command ID
  auto result{Utils::serialize(buf, size, id)};
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the trajectory header
  result = Utils::serialize(buf + bytesWritten, size - bytesWritten, header.seqNum);
  if (result < 0) return false;
  bytesWritten += result;

  result = Utils::serialize(buf + bytesWritten, size - bytesWritten, header.totalParts);
  if (result < 0) return false;
  bytesWritten += result;

  result = Utils::serialize(buf + bytesWritten, size - bytesWritten, header.numEntries);
  if (result < 0) return false;
  bytesWritten += result;

  // Serialize the trajectory entries
  for (std::size_t i = 0; i < header.numEntries; ++i)
  {
    result = Utils::serialize(buf + bytesWritten, size - bytesWritten, entries[i].t);
    if (result < 0) return false;
    bytesWritten += result;

    result = Utils::serialize(buf + bytesWritten, size - bytesWritten, entries[i].az);
    if (result < 0) return false;
    bytesWritten += result;

    result = Utils::serialize(buf + bytesWritten, size - bytesWritten, entries[i].el);
    if (result < 0) return false;
    bytesWritten += result;
  }

  return true;
}

bool PlanTrajectoryCmd::deserializeCommand(const uint8_t* buf, const std::size_t& size)
{
  std::size_t bytesRead{0};

  // Deserialize the command ID
  auto result{Utils::deserialize(buf, size, &id)};
  if (result < 0) return false;
  bytesRead += result;

  // Deserialize the trajectory header
  result = Utils::deserialize(buf + bytesRead, size - bytesRead, &header.seqNum);
  if (result < 0) return false;
  bytesRead += result;

  result = Utils::deserialize(buf + bytesRead, size - bytesRead, &header.totalParts);
  if (result < 0) return false;
  bytesRead += result;

  result = Utils::deserialize(buf + bytesRead, size - bytesRead, &header.numEntries);
  if (result < 0) return false;
  bytesRead += result;

  // Deserialize the trajectory entries
  for (std::size_t i = 0; i < header.numEntries; ++i)
  {
    result = Utils::deserialize(buf + bytesRead, size - bytesRead, &entries[i].t);
    if (result < 0) return false;
    bytesRead += result;

    result = Utils::deserialize(buf + bytesRead, size - bytesRead, &entries[i].az);
    if (result < 0) return false;
    bytesRead += result;

    result = Utils::deserialize(buf + bytesRead, size - bytesRead, &entries[i].el);
    if (result < 0) return false;
    bytesRead += result;
  }

  return true;
}