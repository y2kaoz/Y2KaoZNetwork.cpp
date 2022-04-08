#pragma once

#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <optional>

namespace Y2KaoZ::Network::Protocol::Opcode {

Y2KAOZNETWORK_EXPORT class NoMajorVersion {
public:
  using MajorT = std::size_t;

  constexpr NoMajorVersion() = default;
  auto operator==(const NoMajorVersion&) const -> bool = default;

  explicit constexpr NoMajorVersion(const std::optional<MajorT> /*major*/) {
  }
  [[nodiscard]] constexpr auto major() const noexcept -> std::optional<MajorT> {
    return {};
  }
  constexpr void major(const std::optional<MajorT>& /*major*/) {
  }
};

Y2KAOZNETWORK_EXPORT constexpr inline void byteSerialize(
    [[maybe_unused]] const NoMajorVersion& in,
    [[maybe_unused]] Buffers::VectBufferWriter& out,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!in.major().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void byteDeserialize(
    [[maybe_unused]] NoMajorVersion& out,
    [[maybe_unused]] Buffers::SpanBufferReader& in,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!out.major().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void jsonSerialize(
    [[maybe_unused]] const NoMajorVersion& in,
    [[maybe_unused]] boost::json::object& out) {
  Expects(!in.major().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void jsonDeserialize(
    [[maybe_unused]] NoMajorVersion& out,
    [[maybe_unused]] const boost::json::object& in) {
  Expects(!out.major().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline auto operator<<(std::ostream& out, const NoMajorVersion& /*in*/)
    -> std::ostream& {
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Opcode