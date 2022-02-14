#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"

namespace Y2KaoZ::Network::Buffers {

SpanBufferReader::SpanBufferReader(std::span<const std::byte> buffer, std::size_t start)
  : buffer_(buffer)
  , readed_(start) {
  Expects(readed_ <= buffer_.size());
}

auto SpanBufferReader::readed() const noexcept -> std::size_t {
  Expects(readed_ <= buffer_.size());
  return readed_;
}

auto SpanBufferReader::available() const -> std::size_t {
  return buffer_.size() - readed();
}

void SpanBufferReader::seek(std::size_t index) {
  Expects(index <= buffer_.size());
  readed_ = index;
  Ensures(readed_ <= buffer_.size());
}

} // namespace Y2KaoZ::Network::Buffers