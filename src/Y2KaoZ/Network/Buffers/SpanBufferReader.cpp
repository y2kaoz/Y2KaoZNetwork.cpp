#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"

namespace Y2KaoZ::Network::Buffers {

SpanBufferReader::SpanBufferReader(std::span<const std::byte> buffer, int start) : buffer_(buffer), readed_(start) {
  Expects(readed_ >= 0 && readed_ <= gsl::narrow<int>(buffer_.size()));
}

auto SpanBufferReader::readed() const noexcept -> int {
  Expects(readed_ >= 0 && readed_ <= gsl::narrow<int>(buffer_.size()));
  return readed_;
}

auto SpanBufferReader::available() const -> int {
  return gsl::narrow<int>(buffer_.size()) - readed();
}

} // namespace Y2KaoZ::Network::Buffers