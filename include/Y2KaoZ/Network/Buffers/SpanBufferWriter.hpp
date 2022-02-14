#pragma once

#include "Y2KaoZ/Network/Buffers/NotEnoughSpace.hpp"
#include "Y2KaoZ/Network/Concepts/TriviallyCopyableStandardLayout.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
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
  auto write(const Container& container) -> std::size_t {
    const auto size = sizeof(typename Container::value_type) * container.size();
    if (size > 0) {
      if (available() < size) {
        throw NotEnoughSpace(size - available());
      }
      std::memcpy(&buffer_[written_], std::data(container), size);
      written_ += size;
      Ensures(written_ <= buffer_.size());
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  auto write(const Type& value) -> std::size_t {
    const auto size = sizeof(value);
    if (size > 0) {
      if (available() < size) {
        throw NotEnoughSpace(size - available());
      }
      std::memcpy(&buffer_[written_], &value, size);
      written_ += size;
      Ensures(written_ <= buffer_.size());
    }
    return size;
  }

private:
  std::span<std::byte> buffer_;
  std::size_t written_;
};

Y2KAOZNETWORK_EXPORT template <typename T>
requires TriviallyCopyableStandardLayoutType<T> || TriviallyCopyableStandardLayoutContainer<T>
inline auto operator<<(SpanBufferWriter& buffer, const T& value) -> SpanBufferWriter& {
  buffer.write(value);
  return buffer;
}

} // namespace Y2KaoZ::Network::Buffers