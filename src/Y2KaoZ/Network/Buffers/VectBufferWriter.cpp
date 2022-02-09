#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"

namespace Y2KaoZ::Network::Buffers {

VectBufferWriter::VectBufferWriter(gsl::not_null<std::vector<std::byte>*> vector, std::size_t start)
  : vector_(vector)
  , written_(start) {
  Expects(written_ <= vector_->size());
}

auto VectBufferWriter::written() const noexcept -> std::size_t {
  Expects(written_ <= vector_->size());
  return written_;
}

auto VectBufferWriter::available() const -> std::size_t {
  return vector_->size() - written();
}

} // namespace Y2KaoZ::Network::Buffers