#pragma once

#include "Y2KaoZ/Network/Protocol/Header/BaseHeader.hpp"

namespace Y2KaoZ::Network::Protocol::Header {

template <class AnyHeader_>
concept AnyHeaderT = ByteSerializable<AnyHeader_> && JsonSerializable<AnyHeader_> && requires(
    AnyHeader_ header,
    typename AnyHeader_::OpcodeT opcode,
    std::optional<typename AnyHeader_::HeaderIdT> id,
    std::optional<typename AnyHeader_::BodySizeT> bodySize) {

  Opcode::AnyOpcodeT<typename AnyHeader_::OpcodeT>;
  typename AnyHeader_::HeaderIdT;
  std::unsigned_integral<typename AnyHeader_::BodySizeT>;

  { AnyHeader_(opcode, id, bodySize) } -> std::same_as<AnyHeader_>;
  { AnyHeader_::REJECTED } -> std::convertible_to<typename AnyHeader_::OpcodeT>;

  { header.opcode() } -> std::same_as<const typename AnyHeader_::OpcodeT&>;
  { header.opcode(opcode) } -> std::same_as<void>;
  { header.id() } -> std::same_as<std::optional<typename AnyHeader_::HeaderIdT>>;
  { header.id(id) } -> std::same_as<void>;
  { header.bodySize() } -> std::same_as<std::optional<typename AnyHeader_::BodySizeT>>;
  { header.bodySize(bodySize) } -> std::same_as<void>;
};

static_assert(AnyHeaderT<BaseHeader<
                  Opcode::BaseOpcode<std::uint16_t, Opcode::NoMajorVersion, Opcode::NoMinorVersion>,
                  NoHeaderId,
                  NoBodySize>>);

} // namespace Y2KaoZ::Network::Protocol::Header