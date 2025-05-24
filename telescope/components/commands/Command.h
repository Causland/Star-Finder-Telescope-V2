#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Serializer.h"

static constexpr std::size_t MAX_CMD_SIZE{128};

class Command : public Serializer<MAX_CMD_SIZE>
{
public:
  enum CommandID
  {
    CMD_TELEM_RATE,
    CMD_PLAN_TRAJECTORY,
    CMD_CONTROL_CAMERA,
  };
};

#endif