#pragma once

#include "Y2KaoZ/Network/Buffers/Endian.hpp"
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
  void seek(std::size_t index);

  template <TriviallyCopyableStandardLayoutContainer Container>
  auto write(const Container& container, const std::endian& endian = std::endian::native) -> std::size_t {
    using ContainerValue = typename Container::value_type;
    const auto size = sizeof(ContainerValue) * container.size();
    if (size > 0) {
      if (available() < size) {
        vector_->resize(vector_->size() + size);
      }
      if (endian == std::endian::native || sizeof(ContainerValue) == 1) {
        std::memcpy(&vector_->at(written_), std::data(container), size);
        written_ += size;
      } else {
        for (const auto& value : container) {
          write(value, endian);
        }
      }
      Ensures(written_ <= vector_->size());
    }
    return size;
  }

  template <TriviallyCopyableStandardLayoutType Type>
  auto write(const Type& value, const std::endian& endian = std::endian::native) -> std::size_t {
    constexpr const std::size_t size = sizeof(value);
    if constexpr (size > 0) {
      if (available() < size) {
        vector_->resize(vector_->size() + size);
      }
      if (endian == std::endian::native || sizeof(Type) == 1) {
        std::memcpy(&vector_->at(written_), &value, size);
      } else {
        auto copy = toEndian(value, endian);
        std::memcpy(&vector_->at(written_), &copy, size);
      }
      written_ += size;
      Ensures(written_ <= vector_->size());
    }
    return size;
  }

private:
  std::vector<std::byte>* vector_;
  std::size_t written_;
};

Y2KAOZNETWORK_EXPORT template <typename T, std::endian ENDIAN = std::endian::native>
requires TriviallyCopyableStandardLayoutType<T> || TriviallyCopyableStandardLayoutContainer<T>
auto operator<<(VectBufferWriter& buffer, const T& value) -> VectBufferWriter& {
  buffer.write<T>(value, ENDIAN);
  return buffer;
}

} // namespace Y2KaoZ::Network::Buffers