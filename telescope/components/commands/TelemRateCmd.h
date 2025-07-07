#ifndef __TELEM_RATE_CMD_H__
#define __TELEM_RATE_CMD_H__

#include "Command.h"

class TelemRateCmd : public Command
{
public:
  TelemRateCmd();
  explicit TelemRateCmd(const uint16_t rate);

  bool serializeCommand() override;
  bool deserializeCommand() override;

  uint16_t rate; ///< Telemetry rate in milliseconds
};

#endif // __TELEM_RATE_CMD_H__