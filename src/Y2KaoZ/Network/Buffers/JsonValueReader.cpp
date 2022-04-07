#include "Y2KaoZ/Network/Buffers/JsonValueReader.hpp"
#include <boost/json/kind.hpp>
#include <gsl/gsl_assert>
#include <gsl/gsl_util>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {

auto string(boost::json::string value) -> std::string {
  return {value.data(), value.size()};
}

auto string(boost::json::string_view value) -> std::string {
  return {value.data(), value.size()};
}

auto array(boost::json::array value) -> std::vector<boost::json::value> {
  return {value.begin(), value.end()};
}

} // namespace

namespace Y2KaoZ::Network::Buffers {

// TODO(y2kaoz): investigate and report boost::json::value has an unknown bug in copy constructor.
// JsonValueReader::JsonValueReader(const boost::json::value& value) : value_{value} {
//   Expects(value_ == value);
// }

JsonValueReader::JsonValueReader(const boost::json::value& value) {
  // TODO(y2kaoz): investigate and report boost::json::value has an unknown bug in copy constructor.
  value_ = value; // NOLINT(cppcoreguidelines-prefer-member-initializer)
  Expects(value_ == value);
}

[[nodiscard]] auto JsonValueReader::value() const noexcept -> const boost::json::value& {
  return value_;
}

void JsonValueReader::value(const boost::json::value& value) {
  value_ = value;
}

auto JsonValueReader::kind() const noexcept -> boost::json::kind {
  return value_.kind();
}

auto JsonValueReader::object() const -> boost::json::object {
  const auto k = value_.kind();
  if (k == boost::json::kind::object) {
    return value_.as_object();
  }

  throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as object");
}

auto JsonValueReader::array() const -> std::vector<boost::json::value> {
  const auto k = value_.kind();
  if (k == boost::json::kind::array) {
    return ::array(value_.as_array());
  }

  throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as array");
}

auto JsonValueReader::string() const -> std::string {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? "true" : "false";
    case boost::json::kind::int64:
      return std::to_string(value_.as_int64());
    case boost::json::kind::uint64:
      return std::to_string(value_.as_uint64());
    case boost::json::kind::double_:
      return std::to_string(value_.as_double());
    case boost::json::kind::string:
      return ::string(value_.as_string());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as string");
  };
}

auto JsonValueReader::int8() const -> std::int8_t {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1 : 0;
    case boost::json::kind::int64:
      return gsl::narrow<std::int8_t>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<std::int8_t>(value_.as_uint64());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as int8");
  };
}

auto JsonValueReader::int16() const -> std::int16_t {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1 : 0;
    case boost::json::kind::int64:
      return gsl::narrow<std::int16_t>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<std::int16_t>(value_.as_uint64());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as int16");
  };
}

auto JsonValueReader::int32() const -> std::int32_t {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1 : 0;
    case boost::json::kind::int64:
      return gsl::narrow<std::int32_t>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<std::int32_t>(value_.as_uint64());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as int32");
  };
}

auto JsonValueReader::int64() const -> std::int64_t {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1 : 0;
    case boost::json::kind::int64:
      return gsl::narrow<std::int64_t>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<std::int64_t>(value_.as_uint64());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as int64");
  };
}

auto JsonValueReader::uint8() const -> std::uint8_t {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1 : 0;
    case boost::json::kind::int64:
      return gsl::narrow<std::uint8_t>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<std::uint8_t>(value_.as_uint64());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as uint8");
  };
}

auto JsonValueReader::uint16() const -> std::uint16_t {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1 : 0;
    case boost::json::kind::int64:
      return gsl::narrow<std::uint16_t>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<std::uint16_t>(value_.as_uint64());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as uint16");
  };
}

auto JsonValueReader::uint32() const -> std::uint32_t {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1 : 0;
    case boost::json::kind::int64:
      return gsl::narrow<std::uint32_t>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<std::uint32_t>(value_.as_uint64());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as uint32");
  };
}

auto JsonValueReader::uint64() const -> std::uint64_t {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1 : 0;
    case boost::json::kind::int64:
      return gsl::narrow<std::uint64_t>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<std::uint64_t>(value_.as_uint64());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as uint64");
  };
}

auto JsonValueReader::realDouble() const -> double {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1.0 : 0.0;
    case boost::json::kind::int64:
      return gsl::narrow<double>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<double>(value_.as_uint64());
    case boost::json::kind::double_:
      return value_.as_double();
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as double");
  };
}

auto JsonValueReader::realFloat() const -> float {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool() ? 1.0F : 0.0F;
    case boost::json::kind::int64:
      return gsl::narrow<float>(value_.as_int64());
    case boost::json::kind::uint64:
      return gsl::narrow<float>(value_.as_uint64());
    case boost::json::kind::double_:
      return static_cast<float>(value_.as_double());
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as float");
  };
}

auto JsonValueReader::boolean() const -> bool {
  const auto k = value_.kind();
  switch (k) {
    case boost::json::kind::bool_:
      return value_.as_bool();
    case boost::json::kind::int64:
      return value_.as_int64() != 0;
    case boost::json::kind::uint64:
      return value_.as_uint64() != 0;
    case boost::json::kind::double_:
      return value_.as_double() != 0.0;
    case boost::json::kind::string: {
      const auto s = value_.as_string();
      if (s == "false" || s == "0" || s == "no") {
        return false;
      };
      if (s == "true" || s == "1" || s == "yes") {
        return true;
      };
      [[fallthrough]];
    }
    default:
      throw std::runtime_error("Unable to represent '" + ::string(boost::json::to_string(kind())) + "' as bool");
  };
}

auto JsonValueReader::isNull() const -> bool {
  return value_.is_null();
}

auto JsonValueReader::isObjectContaining(std::string_view key) const -> bool {
  return value_.is_object() && value_.as_object().contains({key.data(), key.size()});
}

} // namespace Y2KaoZ::Network::Buffers