#include "Y2KaoZ/Network/Buffers/JsonBufferReader.hpp"
#include <boost/json/kind.hpp>
#include <gsl/gsl_util>
#include <utility>

namespace {} // namespace

namespace Y2KaoZ::Network::Buffers {

JsonBufferReader::JsonBufferReader(const std::string_view& buffer, std::size_t start)
    : readed_(start)
    , available_(buffer.size() >= readed_ ? buffer.size() - readed_ : 0) {
  Expects(readed_ <= buffer.size());
  Expects(available_ <= buffer.size());
  const auto substr = buffer.substr(readed_);
  if (substr.empty()) {
    return;
  }
  boost::json::stream_parser parser;
  std::size_t readed = parser.write_some(substr.data(), substr.size());
  if (!parser.done()) {
    throw Y2KaoZ::Network::Buffers::NotEnoughData(0);
  }
  valueReader_.value(parser.release());
  readed_ = readed;
  available_ = buffer.size() - readed_;
}

auto JsonBufferReader::readed() const noexcept -> std::size_t {
  return readed_;
}

auto JsonBufferReader::available() const -> std::size_t {
  return available_;
}

auto JsonBufferReader::valueReader() const -> const JsonValueReader& {
  return valueReader_;
}

} // namespace Y2KaoZ::Network::Buffers