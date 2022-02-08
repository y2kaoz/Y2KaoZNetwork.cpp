#pragma once

#include "Y2KaoZ/Network/Concepts/TriviallyCopyableStandardLayout.hpp"
#include <cstring>
#include <gsl/gsl_assert>
#include <gsl/gsl_util>
#include <gsl/pointers>
#include <span>
#include <stdexcept>
#include <vector>

namespace Y2KaoZ::Network::Buffers {

using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutContainer;
using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutType;

class VectBufferWriter {
  explicit VectBufferWriter(gsl::not_null<std::vector<std::byte>*> vector, int start = 0);
  [[nodiscard]] auto written() const noexcept -> int;
  [[nodiscard]] auto available() const -> int;

  template <TriviallyCopyableStandardLayoutContainer Container>
  auto write(const Container& container) -> int {
    const int size = gsl::narrow<int>(sizeof(Container::value_type) * container.size());
    if (size > 0) {
      if (available() < size) {
        vector_->resize(vector_->size() + size);
      }
      std::memcpy(&vector_->at(written_), std::data(container), size);
      written_ += size;
      Ensures(written_ <= gsl::narrow<int>(vector_->size()));
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutContainer Container>
  auto operator<<(const Container& container) -> VectBufferWriter& {
    write(container);
    return *this;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  auto write(const Type& value) -> int {
    const int size = gsl::narrow<int>(sizeof(value));
    if (size > 0) {
      if (available() < size) {
        vector_->resize(vector_->size() + size);
      }
      std::memcpy(&vector_->at(written_), &value, size);
      written_ += size;
      Ensures(written_ <= gsl::narrow<int>(vector_->size()));
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  auto operator<<(const Type& value) -> VectBufferWriter& {
    write(value);
    return *this;
  }

private:
  std::vector<std::byte>* vector_;
  int written_;
};

} // namespace Y2KaoZ::Network::Buffers