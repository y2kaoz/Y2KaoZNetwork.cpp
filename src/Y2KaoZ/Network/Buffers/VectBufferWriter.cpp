#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"

namespace Y2KaoZ::Network::Buffers {

VectBufferWriter::VectBufferWriter(gsl::not_null<std::vector<std::byte>*> vector, int start)
  : vector_(vector)
  , written_(start) {
  Expects(written_ >= 0 && written_ <= gsl::narrow<int>(vector_->size()));
}

auto VectBufferWriter::written() const noexcept -> int {
  Expects(written_ <= gsl::narrow<int>(vector_->size()));
  return written_;
}

auto VectBufferWriter::available() const -> int {
  return gsl::narrow<int>(vector_->size()) - written();
}

} // namespace Y2KaoZ::Network::Buffers