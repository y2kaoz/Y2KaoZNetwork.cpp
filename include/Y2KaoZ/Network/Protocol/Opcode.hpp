#pragma once

#include "Opcode/AnyOpcodeT.hpp"

namespace Y2KaoZ::Network::Protocol::Opcode {

using TinyOpcode = BaseOpcode<std::uint8_t, NoMajorVersion, NoMinorVersion>;
using SmallOpcode = BaseOpcode<std::uint8_t, MajorVersion<std::uint8_t>, NoMinorVersion>;
using MediumOpcode = BaseOpcode<std::uint16_t, MajorVersion<std::uint8_t>, MinorVersion<std::uint8_t>>;
using BigOpcode = BaseOpcode<std::uint32_t, MajorVersion<std::uint16_t>, MinorVersion<std::uint16_t>>;

static_assert(AnyOpcodeT<TinyOpcode>); // 1
static_assert(AnyOpcodeT<SmallOpcode>); // 2
static_assert(AnyOpcodeT<MediumOpcode>); // 4
static_assert(AnyOpcodeT<BigOpcode>); // 8

} // namespace Y2KaoZ::Network::Protocol::Opcode