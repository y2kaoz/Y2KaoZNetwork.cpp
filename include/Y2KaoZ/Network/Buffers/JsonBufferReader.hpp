#pragma once

#include "Y2KaoZ/Network/Buffers/NotEnoughData.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/json/object.hpp>
#include <boost/json/serialize.hpp>
#include <boost/json/stream_parser.hpp>

namespace Y2KaoZ::Network::Buffers {

class Y2KAOZNETWORK_EXPORT JsonBufferReader {
public:
  explicit JsonBufferReader(const std::string_view& buffer, std::size_t start = 0);
  [[nodiscard]] auto readed() const noexcept -> std::size_t;
  [[nodiscard]] auto available() const -> std::size_t;
  [[nodiscard]] auto kind() const noexcept -> boost::json::kind;

  [[nodiscard]] auto object() const -> boost::json::object;
  [[nodiscard]] auto array() const -> std::vector<boost::json::value>;
  [[nodiscard]] auto string() const -> std::string;
  [[nodiscard]] auto int8() const -> std::int8_t;
  [[nodiscard]] auto int16() const -> std::int16_t;
  [[nodiscard]] auto int32() const -> std::int32_t;
  [[nodiscard]] auto int64() const -> std::int64_t;
  [[nodiscard]] auto uint8() const -> std::uint8_t;
  [[nodiscard]] auto uint16() const -> std::uint16_t;
  [[nodiscard]] auto uint32() const -> std::uint32_t;
  [[nodiscard]] auto uint64() const -> std::uint64_t;
  [[nodiscard]] auto realDouble() const -> double;
  [[nodiscard]] auto realFloat() const -> float;
  [[nodiscard]] auto boolean() const -> bool;

  [[nodiscard]] auto isNull() const -> bool;
  [[nodiscard]] auto isObjectContaining(std::string_view key) const -> bool;

private:
  boost::json::value value_{};
  std::size_t readed_{};
  std::size_t available_{};
};

} // namespace Y2KaoZ::Network::Buffers