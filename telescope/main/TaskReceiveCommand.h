#ifndef __TASK_RECEIVE_COMMAND_H__
#define __TASK_RECEIVE_COMMAND_H__

#include <array>
#include <chrono>

#include "Command.h"
#include "CustomTask.h"
#include "NetworkConstants.h"
#include "Telemetry.h"
#include "UDPReceiver.h"

class TaskReceiveCommand : public CustomTask
{
public:
  TaskReceiveCommand(Telemetry& telemetry, const esp_pthread_cfg_t& threadConfig);

  TaskReceiveCommand(const TaskReceiveCommand&) = delete;
  TaskReceiveCommand(TaskReceiveCommand&&) = delete;

  ~TaskReceiveCommand() override = default;

  TaskReceiveCommand& operator=(const TaskReceiveCommand&) = delete;
  TaskReceiveCommand& operator=(TaskReceiveCommand&&) = delete;

  static uint16_t getCmdsReceived() { return cmdsReceived; }

protected:
  void threadLoop() override;

private:
  UDPReceiver cmdReceiver{"CmdReceiver", WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT}; ///< UDP receiver for commands
  std::array<uint8_t, Command::MAX_CMD_SIZE> cmdBuffer{}; ///< Buffer to hold serialized commands

  static uint16_t cmdsReceived; ///< Counter for commands received, registered in telemetry

  static constexpr std::chrono::milliseconds RECEIVE_PERIOD_MS{50}; ///< Period to check for incoming commands
};

#endif // __TASK_RECEIVE_COMMAND_H__