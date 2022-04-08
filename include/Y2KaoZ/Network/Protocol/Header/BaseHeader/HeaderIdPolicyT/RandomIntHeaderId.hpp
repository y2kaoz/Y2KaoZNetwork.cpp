#pragma once

#include "Y2KaoZ/Network/Buffers/JsonValueReader.hpp"
#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <bit>
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <optional>
#include <random>

namespace Y2KaoZ::Network::Protocol::Header {

template <std::unsigned_integral HeaderIdT_ = std::size_t>
Y2KAOZNETWORK_EXPORT class RandomIntHeaderId {
public:
  using HeaderIdT = HeaderIdT_;
  auto operator==(const RandomIntHeaderId&) const -> bool = default;
  explicit RandomIntHeaderId(const std::optional<HeaderIdT>& id = {}) : id_(id.value_or(0)) {
    if (!id.has_value()) {
      generate();
    }
  }
  [[nodiscard]] constexpr auto id() const noexcept -> std::optional<HeaderIdT> {
    return id_;
  }
  auto generate() -> HeaderIdT {
    static class {
    public:
      void operator()(std::span<std::uint8_t> out) {
        std::lock_guard<std::mutex> lock_guard(mutex);
        for (auto& byte : out) {
          byte = static_cast<std::uint8_t>(distribution(rng));
        }
      }

    private:
      std::mutex mutex{};
      std::uniform_int_distribution<std::mt19937::result_type> distribution{0, static_cast<std::uint8_t>(-1)};
      std::mt19937 rng{(std::random_device{})()};
    } generator;
    auto array = std::bit_cast<std::array<std::uint8_t, sizeof(HeaderIdT)>>(id_);
    generator(array);
    id_ = std::bit_cast<HeaderIdT>(array);
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
    const RandomIntHeaderId<HeaderIdT_>& in,
    Buffers::VectBufferWriter& out,
    std::endian endian = std::endian::native) {
  Expects(in.id().has_value());
  out.write(in.id().value(), endian);
}

template <std::unsigned_integral HeaderIdT_>
Y2KAOZNETWORK_EXPORT void byteDeserialize(
    RandomIntHeaderId<HeaderIdT_>& out,
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
Y2KAOZNETWORK_EXPORT inline void jsonSerialize(const RandomIntHeaderId<HeaderIdT_>& in, boost::json::object& out) {
  Expects(in.id().has_value());
  out["id"] = in.id().value();
}

template <std::unsigned_integral HeaderIdT_>
Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(RandomIntHeaderId<HeaderIdT_>& out, const boost::json::object& in) {
  Expects(out.id().has_value());
  out.id(gsl::narrow<HeaderIdT_>(Buffers::JsonValueReader{in, "id"}.uint64()));
}

template <std::unsigned_integral HeaderIdT_>
Y2KAOZNETWORK_EXPORT inline auto operator<<(std::ostream& out, const RandomIntHeaderId<HeaderIdT_>& in)
    -> std::ostream& {
  boost::json::object buffer{};
  jsonSerialize(in, buffer);
  out << boost::json::serialize(buffer);
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Header