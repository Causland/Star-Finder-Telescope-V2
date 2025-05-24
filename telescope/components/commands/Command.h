#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <array>

class Command
{
public:
  static constexpr uint16_t MAX_CMD_SIZE{128};

  enum CommandID
  {
    CMD_TELEM_RATE,
    CMD_PLAN_TRAJECTORY,
    CMD_CONTROL_CAMERA,
  };

  template <typename T>
  bool serialize(const T& obj);

  template <typename T>
  bool deserialize(T* obj);

  void reset() 
  {
    writeOff = 0;
    readOff = 0;
  }

private:
  std::array<uint8_t, MAX_CMD_SIZE> buffer;
  size_t writeOff{0};
  size_t readOff{0};
};

template <typename T>
bool Command::serialize(const T& obj)
{
  if (buffer.size() < writeOff + sizeof(T))
  {
    return false;
  }

  // Convert to network byte order
  const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&obj);
  for (size_t i = 0; i < sizeof(T); ++i)
  {
    buffer[i + writeOff] = ptr[sizeof(T) - 1 - i];
  }
  writeOff += sizeof(T);
  return true;
}

template <typename T>
bool Command::deserialize(T* obj)
{
  if (buffer.size() < sizeof(T) + readOff)
  {
      return false;
  }

  // Convert from network byte order
  const uint8_t* ptr = reinterpret_cast<const uint8_t*>(buffer);
  for (size_t i = 0; i < sizeof(T); ++i)
  {
      ((uint8_t*)obj)[sizeof(T) - 1 - i] = ptr[i + readOff];
  }
  readOff += sizeof(T);
  return true;
}

#endif