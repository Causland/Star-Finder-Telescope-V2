#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include <array>
#include <cstdint>

/// The Serializer class provides a simple abstraction for a serializable
/// object that can be serialized and deserialized to/from a buffer.
/// It uses a fixed-size buffer and supports serialization of any object via memcpy.
template <std::size_t BUF_SIZE>
class Serializer
{
public:
  /// Serialize the object into the buffer and byte swaps into network byte order.
  ///
  /// @param[in] obj The object to serialize
  ///
  /// @return true if serialization was successful, false if buffer does not have enough space
  template <typename T>
  bool serialize(const T& obj);

  /// Deserialize the object from the buffer and byte swaps from network byte order.
  ///
  /// @param[out] obj The object to deserialize into
  ///
  /// @return true if deserialization was successful, false if buffer does not have enough data
  template <typename T>
  bool deserialize(T* obj);

  /// Reset the serializer's internal state, clearing the write and read offsets.
  void reset() 
  {
    writeOff = 0;
    readOff = 0;
  }

  /// Set the underlying buffer. Useful for deserializing from a byte stream.
  void setBuffer(std::array<uint8_t, BUF_SIZE>&& buf)
  {
    buffer = std::move(buf);
    reset();
  }

  const std::array<uint8_t, BUF_SIZE>& getBuffer() const { return buffer; }
  std::size_t getBytesWritten() const { return writeOff; }
  std::size_t getBytesRead() const { return readOff; }

private:
  std::array<uint8_t, BUF_SIZE> buffer; ///< Fixed-size buffer for serialization
  std::size_t writeOff{0}; ///< Offset for writing to the buffer
  std::size_t readOff{0}; ///< Offset for reading from the buffer
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
  for (std::size_t i = 0; i < sizeof(T); ++i)
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
  for (std::size_t i = 0; i < sizeof(T); ++i)
  {
      ((uint8_t*)obj)[sizeof(T) - 1 - i] = ptr[i + readOff];
  }
  readOff += sizeof(T);
  return true;
}

#endif