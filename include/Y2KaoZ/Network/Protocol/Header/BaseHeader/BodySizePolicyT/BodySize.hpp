#pragma once

#include "Y2KaoZ/Network/Buffers/JsonValueReader.hpp"
#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <optional>
#include <stdexcept>

namespace Y2KaoZ::Network::Protocol::Header {

template <std::unsigned_integral BodySizeT_ = std::size_t>
Y2KAOZNETWORK_EXPORT class BodySize {
public:
  using BodySizeT = BodySizeT_;

  constexpr BodySize() = default;
  auto operator==(const BodySize&) const -> bool = default;

  explicit constexpr BodySize(const std::optional<BodySizeT>& bodySize) : bodySize_{bodySize.value_or(0)} {
  }
  [[nodiscard]] constexpr auto bodySize() const noexcept -> std::optional<BodySizeT> {
    return bodySize_;
  }
  constexpr void bodySize(const std::optional<BodySizeT>& bodySize) {
    Expects(bodySize.has_value());
    bodySize_ = bodySize.value();
  }

private:
  BodySizeT bodySize_;
};

template <std::unsigned_integral BodySizeT_>
Y2KAOZNETWORK_EXPORT void byteSerialize(
    const BodySize<BodySizeT_>& in,
    Buffers::VectBufferWriter& out,
    std::endian endian = std::endian::native) {
  Expects(in.bodySize().has_value());
  out.write(in.bodySize().value(), endian);
}

template <std::unsigned_integral BodySizeT_>
Y2KAOZNETWORK_EXPORT void byteDeserialize(
    BodySize<BodySizeT_>& out,
    Buffers::SpanBufferReader& in,
    std::endian endian = std::endian::native) {
  Expects(out.bodySize().has_value());
  auto reader = in;

  BodySizeT_ bodySize{};
  reader.read(bodySize, endian);

  out.bodySize(bodySize);
  in.seek(reader.readed());
}

template <std::unsigned_integral BodySizeT_>
Y2KAOZNETWORK_EXPORT inline void jsonSerialize(const BodySize<BodySizeT_>& in, boost::json::object& out) {
  Expects(in.bodySize().has_value());
  out["bodySize"] = in.bodySize().value();
}

template <std::unsigned_integral BodySizeT_>
Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(BodySize<BodySizeT_>& out, const boost::json::object& in) {
  Expects(out.bodySize().has_value());
  out.bodySize(gsl::narrow<BodySizeT_>(Buffers::JsonValueReader{in, "bodySize"}.uint64()));
}

template <std::unsigned_integral BodySizeT_>
Y2KAOZNETWORK_EXPORT inline auto operator<<(std::ostream& out, const BodySize<BodySizeT_>& in) -> std::ostream& {
  boost::json::object buffer{};
  jsonSerialize(in, buffer);
  out << boost::json::serialize(buffer);
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Header