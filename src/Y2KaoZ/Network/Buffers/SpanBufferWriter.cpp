#include "Y2KaoZ/Network/Buffers/SpanBufferWriter.hpp"

namespace Y2KaoZ::Network::Buffers {

SpanBufferWriter::SpanBufferWriter(std::span<std::byte> buffer, std::size_t start) : buffer_(buffer), written_(start) {
  Expects(written_ <= buffer_.size());
}

auto SpanBufferWriter::written() const noexcept -> std::size_t {
  Expects(written_ <= buffer_.size());
  return written_;
}

auto SpanBufferWriter::available() const -> std::size_t {
  return buffer_.size() - written();
}

void SpanBufferWriter::seek(std::size_t index) {
  Expects(index <= buffer_.size());
  written_ = index;
  Ensures(written_ <= buffer_.size());
}

} // namespace Y2KaoZ::Network::Buffers