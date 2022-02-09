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

class Y2KAOZNETWORK_EXPORT SpanBufferReader {
public:
  explicit SpanBufferReader(std::span<const std::byte> buffer, int start = 0);
  [[nodiscard]] auto readed() const noexcept -> int;
  [[nodiscard]] auto available() const -> int;

  template <TriviallyCopyableStandardLayoutContainer Container>
  [[nodiscard]] auto read(Container& container) -> int {
    const int size = gsl::narrow<int>(sizeof(Container::value_type) * container.size());
    if (size > 0) {
      if (available() < size) {
        return available() - size;
      }
      std::memcpy(std::data(container), &buffer_[readed_], size);
      readed_ += size;
      Ensures(readed_ <= gsl::narrow<int>(buffer_.size()));
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  [[nodiscard]] auto availableFor(Type& value) -> int {
    const int size = gsl::narrow<int>(sizeof(value));
    if (size > 0) {
      if (available() < size) {
        return available() - size;
      }
      std::memcpy(&value, &buffer_[readed_], size);
      readed_ += size;
      Ensures(readed_ <= gsl::narrow<int>(buffer_.size()));
    }
    return size;
  }

private:
  std::span<const std::byte> buffer_;
  int readed_;
};

Y2KAOZNETWORK_EXPORT template <typename T>
requires TriviallyCopyableStandardLayoutType<T> || TriviallyCopyableStandardLayoutContainer<T>
inline auto operator>>(SpanBufferReader& buffer, T& value) -> SpanBufferReader& {
  if (buffer.read(value) < 0) {
    throw std::out_of_range("The read is out of the buffer range.");
  }
  return buffer;
}

} // namespace Y2KaoZ::Network::Buffers