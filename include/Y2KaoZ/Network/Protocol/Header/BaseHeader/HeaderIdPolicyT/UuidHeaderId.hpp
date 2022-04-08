#pragma once

#include "Y2KaoZ/Network/Buffers/JsonValueReader.hpp"
#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <optional>

namespace Y2KaoZ::Network::Protocol::Header {

Y2KAOZNETWORK_EXPORT class UuidHeaderId {
public:
  using HeaderIdT = boost::uuids::uuid;
  auto operator==(const UuidHeaderId&) const -> bool = default;
  explicit UuidHeaderId(const std::optional<HeaderIdT>& id = {}) {
    if (!id.has_value()) {
      generate();
    }
  }
  auto generate() -> HeaderIdT {
    static boost::uuids::random_generator generator;
    id_ = generator();
    return id_;
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

Y2KAOZNETWORK_EXPORT inline void byteSerialize(
    const UuidHeaderId& in,
    Buffers::VectBufferWriter& out,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(in.id().has_value());
  out.write(in.id().value().data);
}

Y2KAOZNETWORK_EXPORT inline void byteDeserialize(
    UuidHeaderId& out,
    Buffers::SpanBufferReader& in,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(out.id().has_value());
  auto reader = in;

  using HeaderIdT = typename std::decay_t<decltype(out)>::HeaderIdT;
  HeaderIdT id{};
  reader.read(id.data);

  out.id(id);
  in.seek(reader.readed());
}

Y2KAOZNETWORK_EXPORT inline void jsonSerialize(const UuidHeaderId& in, boost::json::object& out) {
  Expects(in.id().has_value());
  out["id"] = boost::uuids::to_string(in.id().value());
}

Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(UuidHeaderId& out, const boost::json::object& in) {
  Expects(out.id().has_value());
  const auto strId = Buffers::JsonValueReader{in, "id"}.string();
  boost::uuids::string_generator gen;
  out.id(gen(strId.begin(), strId.end()));
}

Y2KAOZNETWORK_EXPORT inline auto operator<<(std::ostream& out, const UuidHeaderId& in) -> std::ostream& {
  boost::json::object buffer{};
  jsonSerialize(in, buffer);
  out << boost::json::serialize(buffer);
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Header