#pragma once

#include "Y2KaoZ/Network/Buffers/JsonValueReader.hpp"
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
  [[nodiscard]] auto valueReader() const -> const JsonValueReader&;

private:
  JsonValueReader valueReader_{};
  std::size_t readed_{};
  std::size_t available_{};
};

} // namespace Y2KaoZ::Network::Buffers