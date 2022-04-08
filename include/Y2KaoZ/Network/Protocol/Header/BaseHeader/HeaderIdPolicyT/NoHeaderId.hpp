#pragma once

#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <cstddef>
#include <optional>

namespace Y2KaoZ::Network::Protocol::Header {

Y2KAOZNETWORK_EXPORT class NoHeaderId {
public:
  using HeaderIdT = std::nullptr_t;

  constexpr NoHeaderId() = default;
  auto operator==(const NoHeaderId&) const -> bool = default;

  explicit constexpr NoHeaderId(const std::optional<HeaderIdT> /*id*/) {
  }
  [[nodiscard]] constexpr auto id() const noexcept -> std::optional<HeaderIdT> {
    return {};
  }
  constexpr void id(const std::optional<HeaderIdT>& /*id*/) {
  }
};

Y2KAOZNETWORK_EXPORT constexpr inline void byteSerialize(
    [[maybe_unused]] const NoHeaderId& in,
    [[maybe_unused]] Buffers::VectBufferWriter& out,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!in.id().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void byteDeserialize(
    [[maybe_unused]] NoHeaderId& out,
    [[maybe_unused]] Buffers::SpanBufferReader& in,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!out.id().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void jsonSerialize(
    [[maybe_unused]] const NoHeaderId& in,
    [[maybe_unused]] boost::json::object& out) {
  Expects(!in.id().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void jsonDeserialize(
    [[maybe_unused]] NoHeaderId& out,
    [[maybe_unused]] const boost::json::object& in) {
  Expects(!out.id().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline auto operator<<(std::ostream& out, const NoHeaderId& /*in*/) -> std::ostream& {
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Header