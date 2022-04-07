#pragma once

#include "Y2KaoZ/Network/Buffers/Endian.hpp"
#include "Y2KaoZ/Network/Buffers/NotEnoughSpace.hpp"
#include "Y2KaoZ/Network/Concepts/TriviallyCopyableStandardLayout.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <cstddef>
#include <cstring>
#include <gsl/gsl_util>
#include <span>
#include <stdexcept>

namespace Y2KaoZ::Network::Buffers {

using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutContainer;
using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutType;

class Y2KAOZNETWORK_EXPORT SpanBufferWriter {
public:
  explicit SpanBufferWriter(std::span<std::byte> buffer, std::size_t start = 0);
  [[nodiscard]] auto written() const noexcept -> std::size_t;
  [[nodiscard]] auto available() const -> std::size_t;
  void seek(std::size_t index);

  template <TriviallyCopyableStandardLayoutContainer Container>
  auto write(const Container& container, const std::endian& endian = std::endian::native) -> std::size_t {
    using ContainerValue = typename Container::value_type;
    const auto size = sizeof(ContainerValue) * container.size();
    if (size > 0) {
      if (available() < size) {
        throw NotEnoughSpace(size - available());
      }
      if (endian == std::endian::native || sizeof(ContainerValue) == 1) {
        std::memcpy(&buffer_[written_], std::data(container), size);
        written_ += size;
      } else {
        for (const auto& value : container) {
          using Type = typename Container::value_type;
          static_assert(TriviallyCopyableStandardLayoutType<Type>);
          write<Type>(value, endian);
        }
      }
      Ensures(written_ <= buffer_.size());
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  auto write(const Type& value, const std::endian& endian = std::endian::native) -> std::size_t {
    constexpr const std::size_t size = sizeof(value);
    if constexpr (size > 0) {
      if (available() < size) {
        throw NotEnoughSpace(size - available());
      }
      if (endian == std::endian::native || sizeof(Type) == 1) {
        std::memcpy(&buffer_[written_], &value, size);
      } else {
        auto copy = toEndian(value, endian);
        std::memcpy(&buffer_[written_], &copy, size);
      }
      written_ += size;
      Ensures(written_ <= buffer_.size());
    }
    return size;
  }

private:
  std::span<std::byte> buffer_;
  std::size_t written_;
};

Y2KAOZNETWORK_EXPORT template <typename T, std::endian ENDIAN = std::endian::native>
requires TriviallyCopyableStandardLayoutType<T> || TriviallyCopyableStandardLayoutContainer<T>
inline auto operator<<(SpanBufferWriter& buffer, const T& value) -> SpanBufferWriter& {
  buffer.write(value, ENDIAN);
  return buffer;
}

} // namespace Y2KaoZ::Network::Buffers