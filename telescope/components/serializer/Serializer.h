#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include <array>
#include <cstdint>

template <std::size_t BUF_SIZE>
class Serializer
{
public:
  template <typename T>
  bool serialize(const T& obj);

  template <typename T>
  bool deserialize(T* obj);

  void reset() 
  {
    writeOff = 0;
    readOff = 0;
  }

  std::size_t getBytesWritten() const { return writeOff; }
  std::size_t getBytesRead() const { return readOff; }

private:
  std::array<uint8_t, BUF_SIZE> buffer;
  std::size_t writeOff{0};
  std::size_t readOff{0}; 
};

template <std::size_t BUF_SIZE>
template <typename T>
bool Serializer<BUF_SIZE>::serialize(const T& obj)
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

template <std::size_t BUF_SIZE>
template <typename T>
bool Serializer<BUF_SIZE>::deserialize(T* obj)
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