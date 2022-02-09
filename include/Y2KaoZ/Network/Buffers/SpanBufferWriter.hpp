#pragma once

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
  explicit SpanBufferWriter(std::span<std::byte> buffer, int start = 0);
  [[nodiscard]] auto written() const noexcept -> int;
  [[nodiscard]] auto available() const -> int;

  template <TriviallyCopyableStandardLayoutContainer Container>
  auto write(const Container& container) -> int {
    const int size = gsl::narrow<int>(sizeof(Container::value_type) * container.size());
    if (size > 0) {
      if (available() < size) {
        return available() - size;
      }
      std::memcpy(&buffer_[written_], std::data(container), size);
      written_ += size;
      Ensures(written_ <= gsl::narrow<int>(buffer_.size()));
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  auto write(const Type& value) -> int {
    const int size = gsl::narrow<int>(sizeof(value));
    if (size > 0) {
      if (available() < size) {
        return available() - size;
      }
      std::memcpy(&buffer_[written_], &value, size);
      written_ += size;
      Ensures(written_ <= gsl::narrow<int>(buffer_.size()));
    }
    return size;
  }

private:
  std::span<std::byte> buffer_;
  int written_;
};

Y2KAOZNETWORK_EXPORT template <typename T>
requires TriviallyCopyableStandardLayoutType<T> || TriviallyCopyableStandardLayoutContainer<T>
inline auto operator<<(SpanBufferWriter& buffer, const T& value) -> SpanBufferWriter& {
  if (buffer.write(value) < 0) {
    throw std::out_of_range("The write is out of the buffer range.");
  }
  return buffer;
}

} // namespace Y2KaoZ::Network::Buffers