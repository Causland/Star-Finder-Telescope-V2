#ifndef __SERIALIZATION_H__
#define __SERIALIZATION_H__

#include <cstdint>

namespace Utils
{
  /// Serialize the object into the buffer and byte swaps into network byte order.
  ///
  /// @param[in] buf The buffer to serialize into
  /// @param[in] size The size of the buffer
  /// @param[in] obj The object to serialize
  ///
  /// @return the number of bytes written if serialization was successful,
  ///         or -1 if the buffer does not have enough space
  template <typename T>
  int serialize(uint8_t* buf, const std::size_t& size, const T& obj)
  {
    if (size < sizeof(T))
    {
      return -1;
    }

    // Convert to network byte order
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&obj);
    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
      buf[i] = ptr[sizeof(T) - 1 - i];
    }

    return sizeof(T);
  }

  /// Deserialize the object from the buffer and byte swaps from network byte order.
  ///
  /// @param[in] buf The buffer to deserialize from
  /// @param[in] size The size of the buffer
  /// @param[out] obj The object to deserialize into
  ///
  /// @return the number of bytes read if deserializeation, 
  ///         or -1 if the buffer does not have enough data
  template <typename T>
  int deserialize(const uint8_t* buf, const std::size_t& size, T* obj)
  {
    if (size < sizeof(T))
    {
        return -1;
    }

    // Convert from network byte order
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(buf);
    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
        reinterpret_cast<char*>(obj)[sizeof(T) - 1 - i] = ptr[i];
    }
      return sizeof(T);
  }
}

#endif