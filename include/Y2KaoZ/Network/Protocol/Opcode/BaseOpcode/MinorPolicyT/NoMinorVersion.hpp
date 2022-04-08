#pragma once

#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <optional>

namespace Y2KaoZ::Network::Protocol::Opcode {

Y2KAOZNETWORK_EXPORT class NoMinorVersion {
public:
  using MinorT = std::size_t;
  constexpr NoMinorVersion() = default;
  explicit constexpr NoMinorVersion(const std::optional<MinorT> /*minor*/) {
  }
  [[nodiscard]] constexpr auto minor() const noexcept -> std::optional<MinorT> {
    return {};
  }
  constexpr void minor(const std::optional<MinorT>& /*minor*/) {
  }
};

Y2KAOZNETWORK_EXPORT constexpr inline void byteSerialize(
    [[maybe_unused]] const NoMinorVersion& in,
    [[maybe_unused]] Buffers::VectBufferWriter& out,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!in.minor().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void byteDeserialize(
    [[maybe_unused]] NoMinorVersion& out,
    [[maybe_unused]] Buffers::SpanBufferReader& in,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!out.minor().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void jsonSerialize(
    [[maybe_unused]] const NoMinorVersion& in,
    [[maybe_unused]] boost::json::object& out) {
  Expects(!in.minor().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void jsonDeserialize(
    [[maybe_unused]] NoMinorVersion& out,
    [[maybe_unused]] const boost::json::object& in) {
  Expects(!out.minor().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline auto operator<<(std::ostream& out, const NoMinorVersion& /*in*/)
    -> std::ostream& {
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Opcode