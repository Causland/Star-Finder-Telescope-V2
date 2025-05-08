#include "Commands.h"
#include "Utils.h"

bool createCmdBuffers(MessageBufferHandle_t gMsgBufferHandles[NUM_TASKS],
                      StaticMessageBuffer_t gMsgBuffer[NUM_TASKS],
                      uint8_t gMsgBufferStorage[NUM_TASKS][MAX_CMD_SIZE])
{
  DEBUG_ENTER("createCmdBuffers()");
  for (int i = 0; i < NUM_TASKS; ++i)
  {
    gMsgBufferHandles[i] = xMessageBufferCreateStatic(MAX_CMD_SIZE,
                                                      gMsgBufferStorage[i],
                                                      &gMsgBuffer[i]);
    if (gMsgBufferHandles[i] == nullptr)
    {
      DEBUG_PRINTLN("Failed to allocate message buffer for task " + String(i) + "!");
      return false;
    }
  }
  DEBUG_EXIT("createCmdBuffers()");
  return true;
}