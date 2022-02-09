#pragma once

#include "Y2KaoZ/Network/Concepts/TriviallyCopyableStandardLayout.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/asio/buffer.hpp>
#include <memory>
#include <vector>

namespace Y2KaoZ::Network::Buffers {

using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutContainer;
using Y2KaoZ::Network::Concepts::TriviallyCopyableStandardLayoutType;

class Y2KAOZNETWORK_EXPORT ReferenceCounted {
public:
  using value_type = boost::asio::const_buffer;
  using const_iterator = const boost::asio::const_buffer*;

  explicit ReferenceCounted(std::vector<std::byte> data);
  [[nodiscard]] auto size() const -> std::size_t;
  [[nodiscard]] auto empty() const -> bool;
  [[nodiscard]] auto begin() const -> const_iterator;
  [[nodiscard]] auto end() const -> const_iterator;

  template <TriviallyCopyableStandardLayoutContainer Container>
  explicit ReferenceCounted(const Container& container)
    : data_{std::make_shared<std::vector<std::byte>>(sizeof(Container::value_type) * container.size())}
    , buffer_(boost::asio::buffer(*data_)) {
    std::memcpy(data_->data(), std::data(container), data_->size());
  }

  template <TriviallyCopyableStandardLayoutType Type>
  explicit ReferenceCounted(const Type& value)
    : data_{std::make_shared<std::vector<std::byte>>(sizeof(value))}
    , buffer_(boost::asio::buffer(*data_)) {
    std::memcpy(data_->data(), &value, data_->size());
  }

private:
  std::shared_ptr<std::vector<std::byte>> data_;
  boost::asio::const_buffer buffer_;
};

} // namespace Y2KaoZ::Network::Buffers