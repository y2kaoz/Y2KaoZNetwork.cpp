#pragma once

#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <optional>

namespace Y2KaoZ::Network::Protocol::Opcode {

template <std::unsigned_integral MajorT_ = std::size_t>
Y2KAOZNETWORK_EXPORT class NoMajorVersion {
public:
  using MajorT = MajorT_;
  constexpr NoMajorVersion() = default;
  explicit constexpr NoMajorVersion(const std::optional<MajorT> /*major*/) {
  }
  [[nodiscard]] constexpr auto major() const noexcept -> std::optional<MajorT> {
    return {};
  }
  constexpr void major(const std::optional<MajorT>& /*major*/) {
  }
};

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT void byteSerialize(
    [[maybe_unused]] const NoMajorVersion<MajorT_>& in,
    [[maybe_unused]] Buffers::VectBufferWriter& out,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!in.major().has_value());
}

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT void byteDeserialize(
    [[maybe_unused]] NoMajorVersion<MajorT_>& out,
    [[maybe_unused]] Buffers::SpanBufferReader& in,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(!out.major().has_value());
}

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT inline void jsonSerialize(
    [[maybe_unused]] const NoMajorVersion<MajorT_>& in,
    [[maybe_unused]] boost::json::object& out) {
  Expects(!in.major().has_value());
}

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(
    [[maybe_unused]] NoMajorVersion<MajorT_>& out,
    [[maybe_unused]] const boost::json::object& in) {
  Expects(!out.major().has_value());
}

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT inline auto operator<<(std::ostream& out, const NoMajorVersion<MajorT_>& /*in*/) -> std::ostream& {
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Opcode