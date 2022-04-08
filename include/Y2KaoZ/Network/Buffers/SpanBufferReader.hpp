#pragma once

#include "Y2KaoZ/Network/Buffers/Endian.hpp"
#include "Y2KaoZ/Network/Buffers/NotEnoughData.hpp"
#include "Y2KaoZ/Network/Concepts/TriviallyCopyableStandardLayout.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <cstring>
#include <gsl/gsl_util>
#include <span>
#include <stdexcept>

namespace Y2KaoZ::Network::Buffers {

using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutContainer;
using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutType;

class Y2KAOZNETWORK_EXPORT SpanBufferReader {
public:
  explicit SpanBufferReader(std::span<const std::byte> buffer, std::size_t start = 0);
  [[nodiscard]] auto readed() const noexcept -> std::size_t;
  [[nodiscard]] auto available() const -> std::size_t;
  void seek(std::size_t index);

  template <TriviallyCopyableStandardLayoutContainer Container>
  inline auto read(Container& container) -> std::size_t {
    using ContainerValue = typename Container::value_type;
    const auto size = sizeof(ContainerValue) * container.size();
    if (size > 0) {
      if (available() < size) {
        throw NotEnoughData(size - available());
      }
      std::memcpy(std::data(container), &buffer_[readed_], size);
      readed_ += size;
      Ensures(readed_ <= buffer_.size());
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  inline auto read(Type& value) -> std::size_t {
    const auto size = sizeof(value);
    if (size > 0) {
      if (available() < size) {
        throw NotEnoughData(size - available());
      }
      std::memcpy(&value, &buffer_[readed_], size);
      readed_ += size;
      Ensures(readed_ <= buffer_.size());
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutContainer Container>
  auto read(const Container& container, const std::endian& endian) -> std::size_t {
    using ContainerValue = typename Container::value_type;
    if (endian == std::endian::native || sizeof(ContainerValue) == 1) {
      return read(container);
    }
    std::size_t size = 0;
    for (const auto& value : container) {
      size += read(value, endian);
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  inline auto read(Type& value, const std::endian& endian) -> std::size_t {
    std::size_t size = read(value);
    if (endian != std::endian::native && sizeof(Type) > 1) {
      toNativeInplace(value, endian);
    }
    return size;
  }

private:
  std::span<const std::byte> buffer_;
  std::size_t readed_;
};

Y2KAOZNETWORK_EXPORT template <typename T, std::endian ENDIAN = std::endian::native>
requires TriviallyCopyableStandardLayoutType<T> || TriviallyCopyableStandardLayoutContainer<T>
inline auto operator>>(SpanBufferReader& buffer, T& value) -> SpanBufferReader& {
  buffer.read(value, ENDIAN);
  return buffer;
}

} // namespace Y2KaoZ::Network::Buffers