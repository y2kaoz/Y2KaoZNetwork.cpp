#pragma once

#include "Header/AnyHeaderT.hpp"
#include "Opcode.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/BodySizePolicyT/BodySize.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/BodySizePolicyT/NoBodySize.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/NoHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/RandomBytesHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/RandomIntHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/UuidHeaderId.hpp"
#include <atomic>

namespace Y2KaoZ::Network::Protocol::Header {

using TinyHeader = BaseHeader<Opcode::TinyOpcode, NoHeaderId, NoBodySize>;
using SmallHeader = BaseHeader<Opcode::SmallOpcode, NoHeaderId, BodySize<std::uint16_t>>;
using MediumHeader = BaseHeader<Opcode::MediumOpcode, NoHeaderId, BodySize<std::uint16_t>>;
using BigHeader = BaseHeader<Opcode::BigOpcode, NoHeaderId, BodySize<std::uint16_t>>;

using TinyUniqueHeader = BaseHeader<Opcode::TinyOpcode, RandomBytesHeaderId<3>, NoBodySize>;
using SmallUniqueHeader = BaseHeader<Opcode::SmallOpcode, RandomBytesHeaderId<4>, BodySize<std::uint16_t>>;
using MediumUniqueHeader = BaseHeader<Opcode::MediumOpcode, RandomBytesHeaderId<8>, BodySize<std::uint16_t>>; // NOLINT
using BigUniqueHeader = BaseHeader<Opcode::BigOpcode, UuidHeaderId, BodySize<std::uint16_t>>;

static_assert(AnyHeaderT<TinyHeader>); // 1 bytes
static_assert(AnyHeaderT<SmallHeader>); // 4 bytes
static_assert(AnyHeaderT<MediumHeader>); // 6 bytes
static_assert(AnyHeaderT<BigHeader>); // 10 bytes

static_assert(AnyHeaderT<TinyUniqueHeader>); // 4
static_assert(AnyHeaderT<SmallUniqueHeader>); // 8
static_assert(AnyHeaderT<MediumUniqueHeader>); // 14
static_assert(AnyHeaderT<BigUniqueHeader>); // 26

} // namespace Y2KaoZ::Network::Protocol::Header