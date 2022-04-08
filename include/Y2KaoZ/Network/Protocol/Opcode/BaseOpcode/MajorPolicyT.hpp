#pragma once

#include "Y2KaoZ/Network/Protocol/Concepts.hpp"
#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode/MajorPolicyT/MajorVersion.hpp"
#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode/MajorPolicyT/NoMajorVersion.hpp"

namespace Y2KaoZ::Network::Protocol::Opcode {

template <class MajorPolicyT_>
concept MajorPolicyT = ByteSerializable<MajorPolicyT_> && JsonSerializable<MajorPolicyT_> &&
    requires(MajorPolicyT_ policy, std::optional<typename MajorPolicyT_::MajorT> major) {
  std::unsigned_integral<typename MajorPolicyT_::MajorT>;
  { MajorPolicyT_(major) } -> std::same_as<MajorPolicyT_>;
  { policy.major() } -> std::same_as<std::optional<typename MajorPolicyT_::MajorT>>;
  { policy.major(major) } -> std::same_as<void>;
};

static_assert(MajorPolicyT<MajorVersion<std::size_t>>);
static_assert(MajorPolicyT<NoMajorVersion>);

} // namespace Y2KaoZ::Network::Protocol::Opcode