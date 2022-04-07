#include "Y2KaoZ/Network/Buffers/JsonBufferReader.hpp"
#include <boost/json/kind.hpp>
#include <gsl/gsl_util>

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
  value_ = parser.release();
  readed_ = readed;
  available_ = buffer.size() - readed_;
}

auto JsonBufferReader::readed() const noexcept -> std::size_t {
  return readed_;
}

auto JsonBufferReader::available() const -> std::size_t {
  return available_;
}

auto JsonBufferReader::kind() const noexcept -> boost::json::kind {
  return value_.kind();
}

auto JsonBufferReader::object() const -> boost::json::object {
  return value_.as_object();
}

auto JsonBufferReader::array() const -> std::vector<boost::json::value> {
  auto jsonArray = value_.as_array();
  return {jsonArray.begin(), jsonArray.end()};
}

auto JsonBufferReader::string() const -> std::string {
  auto jsonString = value_.as_string();
  return {jsonString.begin(), jsonString.end()};
}

auto JsonBufferReader::int8() const -> std::int8_t {
  if (value_.kind() == boost::json::kind::uint64) {
    return gsl::narrow<std::int8_t>(value_.as_uint64());
  }
  return gsl::narrow<std::int8_t>(value_.as_int64());
}

auto JsonBufferReader::int16() const -> std::int16_t {
  if (value_.kind() == boost::json::kind::uint64) {
    return gsl::narrow<std::int16_t>(value_.as_uint64());
  }
  return gsl::narrow<std::int16_t>(value_.as_int64());
}

auto JsonBufferReader::int32() const -> std::int32_t {
  if (value_.kind() == boost::json::kind::uint64) {
    return gsl::narrow<std::int32_t>(value_.as_uint64());
  }
  return gsl::narrow<std::int32_t>(value_.as_int64());
}

auto JsonBufferReader::int64() const -> std::int64_t {
  if (value_.kind() == boost::json::kind::uint64) {
    return gsl::narrow<std::int64_t>(value_.as_uint64());
  }
  return gsl::narrow<std::int64_t>(value_.as_int64());
}

auto JsonBufferReader::uint8() const -> std::uint8_t {
  if (value_.kind() == boost::json::kind::int64) {
    return gsl::narrow<std::uint8_t>(value_.as_int64());
  }
  return gsl::narrow<std::uint8_t>(value_.as_uint64());
}

auto JsonBufferReader::uint16() const -> std::uint16_t {
  if (value_.kind() == boost::json::kind::int64) {
    return gsl::narrow<std::uint16_t>(value_.as_int64());
  }
  return gsl::narrow<std::uint16_t>(value_.as_uint64());
}

auto JsonBufferReader::uint32() const -> std::uint32_t {
  if (value_.kind() == boost::json::kind::int64) {
    return gsl::narrow<std::uint32_t>(value_.as_int64());
  }
  return gsl::narrow<std::uint32_t>(value_.as_uint64());
}

auto JsonBufferReader::uint64() const -> std::uint64_t {
  if (value_.kind() == boost::json::kind::int64) {
    return gsl::narrow<std::uint64_t>(value_.as_int64());
  }
  return gsl::narrow<std::uint64_t>(value_.as_uint64());
}

auto JsonBufferReader::realDouble() const -> double {
  return value_.as_double();
}

auto JsonBufferReader::realFloat() const -> float {
  return static_cast<float>(value_.as_double());
}

auto JsonBufferReader::boolean() const -> bool {
  return value_.as_bool();
}

auto JsonBufferReader::isNull() const -> bool {
  return value_.is_null();
}

auto JsonBufferReader::isObjectContaining(std::string_view key) const -> bool {
  return value_.is_object() && value_.as_object().contains({key.data(), key.size()});
}

} // namespace Y2KaoZ::Network::Buffers