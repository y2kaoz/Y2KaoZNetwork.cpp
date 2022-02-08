#include "Y2KaoZ/Network/Buffers/SpanBufferWriter.hpp"

namespace Y2KaoZ::Network::Buffers {

SpanBufferWriter::SpanBufferWriter(std::span<std::byte> buffer, int start) : buffer_(buffer), written_(start) {
  Expects(written_ >= 0 && written_ <= gsl::narrow<int>(buffer_.size()));
}

auto SpanBufferWriter::written() const noexcept -> int {
  Expects(written_ <= gsl::narrow<int>(buffer_.size()));
  return written_;
}

auto SpanBufferWriter::available() const -> int {
  return gsl::narrow<int>(buffer_.size()) - written();
}

} // namespace Y2KaoZ::Network::Buffers