#pragma once

#include "Y2KaoZ/Network/Buffers/JsonValueReader.hpp"
#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <optional>

namespace Y2KaoZ::Network::Protocol::Header {

template <std::unsigned_integral HeaderIdT_ = std::size_t>
Y2KAOZNETWORK_EXPORT class UnsignedHeaderId {
public:
  using HeaderIdT = HeaderIdT_;
  constexpr UnsignedHeaderId() = default;
  auto operator==(const UnsignedHeaderId&) const -> bool = default;
  explicit constexpr UnsignedHeaderId(const std::optional<HeaderIdT>& id) : id_(id.value_or(0)) {
  }
  [[nodiscard]] constexpr auto id() const noexcept -> std::optional<HeaderIdT> {
    return id_;
  }
  constexpr void id(const std::optional<HeaderIdT>& id) {
    Expects(id.has_value());
    id_ = id.value();
  }

private:
  HeaderIdT id_{};
};

template <std::unsigned_integral HeaderIdT_>
Y2KAOZNETWORK_EXPORT void byteSerialize(
    const UnsignedHeaderId<HeaderIdT_>& in,
    Buffers::VectBufferWriter& out,
    std::endian endian = std::endian::native) {
  Expects(in.id().has_value());
  out.write(in.id().value(), endian);
}

template <std::unsigned_integral HeaderIdT_>
Y2KAOZNETWORK_EXPORT void byteDeserialize(
    UnsignedHeaderId<HeaderIdT_>& out,
    Buffers::SpanBufferReader& in,
    std::endian endian = std::endian::native) {
  Expects(out.id().has_value());
  auto reader = in;

  HeaderIdT_ id{};
  reader.read(id, endian);

  out.id(id);
  in.seek(reader.readed());
}

template <std::unsigned_integral HeaderIdT_>
Y2KAOZNETWORK_EXPORT inline void jsonSerialize(const UnsignedHeaderId<HeaderIdT_>& in, boost::json::object& out) {
  Expects(in.id().has_value());
  out["id"] = in.id().value();
}

template <std::unsigned_integral HeaderIdT_>
Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(UnsignedHeaderId<HeaderIdT_>& out, const boost::json::object& in) {
  Expects(out.id().has_value());
  out.id(gsl::narrow<HeaderIdT_>(Buffers::JsonValueReader{in, "id"}.uint64()));
}

template <std::unsigned_integral HeaderIdT_>
Y2KAOZNETWORK_EXPORT inline auto operator<<(std::ostream& out, const UnsignedHeaderId<HeaderIdT_>& in)
    -> std::ostream& {
  boost::json::object buffer{};
  jsonSerialize(in, buffer);
  out << boost::json::serialize(buffer);
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Header