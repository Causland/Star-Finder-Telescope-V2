#include "Commands.h"
#include "Utils.h"

bool createCmdBuffers(MessageBufferHandle_t gMsgBufferHandles[NUM_TASKS])
{
  DEBUG_ENTER("createCmdBuffers()");
  for (int i = 0; i < NUM_TASKS; ++i)
  {
    gMsgBufferHandles[i] = xMessageBufferCreate(MAX_CMD_SIZE);
    if (gMsgBufferHandles[i] == nullptr)
    {
      DEBUG_PRINTLN("Failed to allocate message buffer for task " + String(i) + "!");
      return false;
    }
  }
  DEBUG_EXIT("createCmdBuffers()");
  return true;
}