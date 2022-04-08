#pragma once

#include "BaseOpcode.hpp"

namespace Y2KaoZ::Network::Protocol::Opcode {

template <class AnyOpcode_>
concept AnyOpcodeT = ByteSerializable<AnyOpcode_> && JsonSerializable<AnyOpcode_> && requires(
    AnyOpcode_ opcode,
    typename AnyOpcode_::OpcodeIdT id,
    std::optional<typename AnyOpcode_::MajorT> major,
    std::optional<typename AnyOpcode_::MinorT> minor) {

  OpcodeIdT<typename AnyOpcode_::OpcodeIdT>;
  std::unsigned_integral<typename AnyOpcode_::MajorT>;
  std::unsigned_integral<typename AnyOpcode_::MinorT>;

  { AnyOpcode_(id, major, minor) } -> std::same_as<AnyOpcode_>;
  { AnyOpcode_::REJECTED } -> std::convertible_to<typename AnyOpcode_::OpcodeIdT>;

  { opcode.id() } -> std::same_as<const typename AnyOpcode_::OpcodeIdT&>;
  { opcode.id(id) } -> std::same_as<void>;
  { opcode.major() } -> std::same_as<std::optional<typename AnyOpcode_::MajorT>>;
  { opcode.major(major) } -> std::same_as<void>;
  { opcode.minor() } -> std::same_as<std::optional<typename AnyOpcode_::MinorT>>;
  { opcode.minor(minor) } -> std::same_as<void>;
};

static_assert(AnyOpcodeT<BaseOpcode<std::uint8_t, NoMajorVersion, NoMinorVersion>>);

} // namespace Y2KaoZ::Network::Protocol::Opcode