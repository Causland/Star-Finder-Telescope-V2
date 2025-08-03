#ifndef __FOCUS_CMD_H__
#define __FOCUS_CMD_H__

#include <cstdint>

#include "Command.h"

/// Command to control the focus of the telescope, allowing for
/// manual and automatic focus adjustments.
class FocusCmd : public Command
{
public:
  enum FocusCmdID : uint8_t
  {
    FOCUS_MANUAL,
    FOCUS_AUTO,
  };

  /// Constructs a FocusCmd object with the focus command ID.
  FocusCmd();

  FocusCmd(const FocusCmd&) = default;
  FocusCmd(FocusCmd&&) = default;

  ~FocusCmd() override = default;

  FocusCmd& operator=(const FocusCmd&) = default;
  FocusCmd& operator=(FocusCmd&&) = default;

  struct FocusManual_t
  {
    float degrees{}; ///< Degrees to rotate focus servo relative to current position
  };

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

  FocusCmdID focusCmdID{}; ///< Specifies the underlying action to perform

  FocusManual_t man{}; ///< Manual focus parameters for adjusting the focus servo.
};

#endif // __FOCUS_CMD_H__