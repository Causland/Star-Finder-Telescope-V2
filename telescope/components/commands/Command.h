#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Serializer.h"

class Command : public Serializer<Command::MAX_CMD_SIZE>
{
public:
  static constexpr std::size_t MAX_CMD_SIZE{128};

  enum CommandID
  {
    CMD_TELEM_RATE,
    CMD_PLAN_TRAJECTORY,
    CMD_CONTROL_CAMERA,
  };
};

#endif