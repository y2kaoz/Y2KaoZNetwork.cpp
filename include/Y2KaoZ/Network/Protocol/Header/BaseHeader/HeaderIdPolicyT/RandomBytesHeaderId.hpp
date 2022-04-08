#pragma once

#include "Y2KaoZ/Network/Buffers/JsonValueReader.hpp"
#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/algorithm/hex.hpp>
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <optional>
#include <random>

namespace Y2KaoZ::Network::Protocol::Header {

template <std::size_t SIZE>
Y2KAOZNETWORK_EXPORT class RandomBytesHeaderId {
public:
  using HeaderIdT = std::array<std::uint8_t, SIZE>;
  auto operator==(const RandomBytesHeaderId&) const -> bool = default;
  explicit RandomBytesHeaderId(const std::optional<HeaderIdT>& id = {}) {
    if (!id.has_value()) {
      generate();
    }
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
    generator(id_);
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

template <std::size_t SIZE>
Y2KAOZNETWORK_EXPORT void byteSerialize(
    const RandomBytesHeaderId<SIZE>& in,
    Buffers::VectBufferWriter& out,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(in.id().has_value());
  out.write(in.id().value());
}

template <std::size_t SIZE>
Y2KAOZNETWORK_EXPORT void byteDeserialize(
    RandomBytesHeaderId<SIZE>& out,
    Buffers::SpanBufferReader& in,
    [[maybe_unused]] std::endian endian = std::endian::native) {
  Expects(out.id().has_value());
  auto reader = in;

  using HeaderIdT = typename std::decay_t<decltype(out)>::HeaderIdT;
  HeaderIdT id{};
  reader.read(id);

  out.id(id);
  in.seek(reader.readed());
}

template <std::size_t SIZE>
Y2KAOZNETWORK_EXPORT inline void jsonSerialize(const RandomBytesHeaderId<SIZE>& in, boost::json::object& out) {
  Expects(in.id().has_value());
  auto id = in.id().value();
  std::string strId;
  boost::algorithm::hex_lower(id.begin(), id.end(), std::back_inserter(strId));
  out["id"] = strId;
}

template <std::size_t SIZE>
Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(RandomBytesHeaderId<SIZE>& out, const boost::json::object& in) {
  Expects(out.id().has_value());
  using HeaderIdT = typename std::decay_t<decltype(out)>::HeaderIdT;
  HeaderIdT id{};

  const auto strId = Buffers::JsonValueReader{in, "id"}.string();
  boost::algorithm::unhex(strId.begin(), strId.end(), id.begin());

  out.id(id);
}

template <std::size_t SIZE>
Y2KAOZNETWORK_EXPORT inline auto operator<<(std::ostream& out, const RandomBytesHeaderId<SIZE>& in) -> std::ostream& {
  boost::json::object buffer{};
  jsonSerialize(in, buffer);
  out << boost::json::serialize(buffer);
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Header