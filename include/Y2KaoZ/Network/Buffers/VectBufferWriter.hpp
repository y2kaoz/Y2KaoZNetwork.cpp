#pragma once

#include "Y2KaoZ/Network/Concepts/TriviallyCopyableStandardLayout.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <cstring>
#include <gsl/gsl_util>
#include <gsl/pointers>
#include <span>
#include <stdexcept>
#include <vector>

namespace Y2KaoZ::Network::Buffers {

using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutContainer;
using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutType;

class Y2KAOZNETWORK_EXPORT VectBufferWriter {
public:
  explicit VectBufferWriter(gsl::not_null<std::vector<std::byte>*> vector, std::size_t start = 0);
  [[nodiscard]] auto written() const noexcept -> std::size_t;
  [[nodiscard]] auto available() const -> std::size_t;

  template <TriviallyCopyableStandardLayoutContainer Container>
  auto write(const Container& container) -> std::size_t {
    const auto size = sizeof(Container::value_type) * container.size();
    if (size > 0) {
      if (available() < size) {
        vector_->resize(vector_->size() + size);
      }
      std::memcpy(&vector_->at(written_), std::data(container), size);
      written_ += size;
      Ensures(written_ <= vector_->size());
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  auto write(const Type& value) -> std::size_t {
    const auto size = sizeof(value);
    if (size > 0) {
      if (available() < size) {
        vector_->resize(vector_->size() + size);
      }
      std::memcpy(&vector_->at(written_), &value, size);
      written_ += size;
      Ensures(written_ <= vector_->size());
    }
    return size;
  }

private:
  std::vector<std::byte>* vector_;
  std::size_t written_;
};

Y2KAOZNETWORK_EXPORT template <typename T>
requires TriviallyCopyableStandardLayoutType<T> || TriviallyCopyableStandardLayoutContainer<T>
auto operator<<(VectBufferWriter& buffer, const T& value) -> VectBufferWriter& {
  buffer.write(value);
  return buffer;
}

} // namespace Y2KaoZ::Network::Buffers