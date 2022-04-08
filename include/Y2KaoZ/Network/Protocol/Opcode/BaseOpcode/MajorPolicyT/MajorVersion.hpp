#pragma once

#include "Y2KaoZ/Network/Buffers/JsonValueReader.hpp"
#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/serialize.hpp>
#include <gsl/gsl_assert>
#include <optional>

namespace Y2KaoZ::Network::Protocol::Opcode {

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT class MajorVersion {
public:
  using MajorT = MajorT_;
  constexpr MajorVersion() = default;
  explicit constexpr MajorVersion(const std::optional<MajorT>& major) : major_{major.value_or(0)} {
  }
  [[nodiscard]] constexpr auto major() const noexcept -> std::optional<MajorT> {
    return major_;
  }
  constexpr void major(const std::optional<MajorT>& major) {
    Expects(major.has_value());
    major_ = major.value();
  }

private:
  MajorT major_{};
};

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT void byteSerialize(
    const MajorVersion<MajorT_>& in,
    Buffers::VectBufferWriter& out,
    std::endian endian = std::endian::native) {
  Expects(in.major().has_value());
  out.write(in.major().value(), endian);
}

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT void byteDeserialize(
    MajorVersion<MajorT_>& out,
    Buffers::SpanBufferReader& in,
    std::endian endian = std::endian::native) {
  Expects(out.major().has_value());
  auto reader = in;

  using MajorT = typename std::decay_t<decltype(out)>::MajorT;
  MajorT major{};
  reader.read(major, endian);

  out.major(major);
  in.seek(reader.readed());
}

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT inline void jsonSerialize(const MajorVersion<MajorT_>& in, boost::json::object& out) {
  Expects(in.major().has_value());
  out["major"] = in.major().value();
}

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(MajorVersion<MajorT_>& out, const boost::json::object& in) {
  Expects(out.major().has_value());
  using MajorT = typename std::decay_t<decltype(out)>::MajorT;
  out.major(gsl::narrow<MajorT>(Buffers::JsonValueReader{in, "major"}.uint64()));
}

template <std::unsigned_integral MajorT_>
Y2KAOZNETWORK_EXPORT inline auto operator<<(std::ostream& out, const MajorVersion<MajorT_>& in) -> std::ostream& {
  boost::json::object buffer{};
  jsonSerialize(in, buffer);
  out << boost::json::serialize(buffer);
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Opcode