#pragma once

#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <optional>

namespace Y2KaoZ::Network::Protocol::Header {

Y2KAOZNETWORK_EXPORT class NoBodySize {
public:
  using BodySizeT = std::size_t;

  constexpr NoBodySize() = default;
  auto operator==(const NoBodySize&) const -> bool = default;

  explicit constexpr NoBodySize(const std::optional<BodySizeT>& /*bodySize*/) {
  }
  [[nodiscard]] constexpr auto bodySize() const noexcept -> std::optional<BodySizeT> {
    return {};
  }
  constexpr void bodySize(const std::optional<BodySizeT>& /*bodySize*/) {
  }
};

Y2KAOZNETWORK_EXPORT constexpr inline void byteSerialize(
    [[maybe_unused]] const NoBodySize& in,
    [[maybe_unused]] Buffers::VectBufferWriter& out,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!in.bodySize().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void byteDeserialize(
    [[maybe_unused]] NoBodySize& out,
    [[maybe_unused]] Buffers::SpanBufferReader& in,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!out.bodySize().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void jsonSerialize(
    [[maybe_unused]] const NoBodySize& in,
    [[maybe_unused]] boost::json::object& out) {
  Expects(!in.bodySize().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline void jsonDeserialize(
    [[maybe_unused]] NoBodySize& out,
    [[maybe_unused]] const boost::json::object& in) {
  Expects(!out.bodySize().has_value());
}

Y2KAOZNETWORK_EXPORT constexpr inline auto operator<<(std::ostream& out, const NoBodySize& /*in*/) -> std::ostream& {
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Header