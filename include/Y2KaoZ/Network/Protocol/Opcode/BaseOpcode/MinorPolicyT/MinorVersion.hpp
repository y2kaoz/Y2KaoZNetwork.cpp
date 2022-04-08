#pragma once

#include "Y2KaoZ/Network/Buffers/JsonValueReader.hpp"
#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/serialize.hpp>
#include <gsl/gsl_assert>
#include <optional>

namespace Y2KaoZ::Network::Protocol::Opcode {

template <std::unsigned_integral MinorT_>
Y2KAOZNETWORK_EXPORT class MinorVersion {
public:
  using MinorT = MinorT_;

  constexpr MinorVersion() = default;
  auto operator==(const MinorVersion&) const -> bool = default;

  explicit constexpr MinorVersion(const std::optional<MinorT>& minor) : minor_{minor.value_or(0)} {
  }
  [[nodiscard]] constexpr auto minor() const noexcept -> std::optional<MinorT> {
    return minor_;
  }
  constexpr void minor(const std::optional<MinorT>& minor) {
    Expects(minor.has_value());
    minor_ = minor.value();
  }

private:
  MinorT minor_{};
};

template <std::unsigned_integral MinorT_>
Y2KAOZNETWORK_EXPORT void byteSerialize(
    const MinorVersion<MinorT_>& in,
    Buffers::VectBufferWriter& out,
    std::endian endian = std::endian::native) {
  Expects(in.minor().has_value());
  out.write(in.minor().value(), endian);
}

template <std::unsigned_integral MinorT_>
Y2KAOZNETWORK_EXPORT void byteDeserialize(
    MinorVersion<MinorT_>& out,
    Buffers::SpanBufferReader& in,
    std::endian endian = std::endian::native) {
  Expects(out.minor().has_value());
  auto reader = in;

  using MinorT = typename std::decay_t<decltype(out)>::MinorT;
  MinorT minor{};
  reader.read(minor, endian);

  out.minor(minor);
  in.seek(reader.readed());
}

template <std::unsigned_integral MinorT_>
Y2KAOZNETWORK_EXPORT inline void jsonSerialize(const MinorVersion<MinorT_>& in, boost::json::object& out) {
  Expects(in.minor().has_value());
  out["minor"] = in.minor().value();
}

template <std::unsigned_integral MinorT_>
Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(MinorVersion<MinorT_>& out, const boost::json::object& in) {
  Expects(out.minor().has_value());
  using MinorT = typename std::decay_t<decltype(out)>::MinorT;
  out.minor(gsl::narrow<MinorT>(Buffers::JsonValueReader{in, "minor"}.uint64()));
}

template <std::unsigned_integral MinorT_>
Y2KAOZNETWORK_EXPORT inline auto operator<<(std::ostream& out, const MinorVersion<MinorT_>& in) -> std::ostream& {
  boost::json::object buffer{};
  jsonSerialize(in, buffer);
  out << boost::json::serialize(buffer);
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Opcode