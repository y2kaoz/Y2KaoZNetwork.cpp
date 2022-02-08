#pragma once

#include "Y2KaoZ/Network/Concepts/TriviallyCopyableStandardLayout.hpp"
#include <cstring>
#include <gsl/gsl_util>
#include <span>
#include <stdexcept>

namespace Y2KaoZ::Network::Buffers {

using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutContainer;
using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutType;

class SpanBufferWriter {
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

  template <TriviallyCopyableStandardLayoutContainer Container>
  auto operator<<(const Container& container) -> SpanBufferWriter& {
    if (write(container) < 0) {
      throw std::out_of_range("The write is out of the buffer range.");
    }
    return *this;
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

  template <TriviallyCopyableStandardLayoutType Type>
  auto operator<<(const Type& value) -> SpanBufferWriter& {
    if (write(value) < 0) {
      throw std::out_of_range("The write is out of the buffer range.");
    }
    return *this;
  }

private:
  std::span<std::byte> buffer_;
  int written_;
};

} // namespace Y2KaoZ::Network::Buffers