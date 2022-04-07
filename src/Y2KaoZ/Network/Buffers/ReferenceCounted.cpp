#include "Y2KaoZ/Network/Buffers/ReferenceCounted.hpp"

namespace Y2KaoZ::Network::Buffers {

ReferenceCounted::ReferenceCounted(std::vector<std::byte> data)
    : data_{std::make_shared<std::vector<std::byte>>(std::move(data))}
    , buffer_(boost::asio::buffer(*data_)) {
}

auto ReferenceCounted::size() const -> std::size_t {
  return data_->size();
}

auto ReferenceCounted::empty() const -> bool {
  return data_->empty();
}

auto ReferenceCounted::begin() const -> const_iterator {
  return &buffer_;
}

auto ReferenceCounted::end() const -> const_iterator {
  return &buffer_ + 1; // NOLINT
}

} // namespace Y2KaoZ::Network::Buffers