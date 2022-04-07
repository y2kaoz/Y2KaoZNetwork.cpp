#pragma once

#include "Y2KaoZ/Network/Buffers/JsonBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
#include "Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
#include <type_traits>

namespace Y2KaoZ::Network::Protocol {

template <class ByteSerializable_>
concept ByteSerializable = requires(
    ByteSerializable_ byteSerializable,
    Buffers::VectBufferWriter writer,
    Buffers::SpanBufferReader reader,
    std::endian endian) {
  { byteSerialize(byteSerializable, writer, endian) } -> std::same_as<void>;
  { byteDeserialize(byteSerializable, reader, endian) } -> std::same_as<void>;
};

template <class JsonSerializable_>
concept JsonSerializable = requires(JsonSerializable_ jsonSerializable, boost::json::object json) {
  { jsonSerialize(jsonSerializable, json) } -> std::same_as<void>;
  { jsonDeserialize(jsonSerializable, json) } -> std::same_as<void>;
};

} // namespace Y2KaoZ::Network::Protocol