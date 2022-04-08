#pragma once

#include "Y2KaoZ/Network/Protocol/Concepts.hpp"
#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode/MinorPolicyT/MinorVersion.hpp"
#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode/MinorPolicyT/NoMinorVersion.hpp"

namespace Y2KaoZ::Network::Protocol::Opcode {

template <class MinorPolicyT_>
concept MinorPolicyT = ByteSerializable<MinorPolicyT_> && JsonSerializable<MinorPolicyT_> &&
    requires(MinorPolicyT_ policy, std::optional<typename MinorPolicyT_::MinorT> minor) {
  std::unsigned_integral<typename MinorPolicyT_::MinorT>;
  { MinorPolicyT_(minor) } -> std::same_as<MinorPolicyT_>;
  { policy.minor() } -> std::same_as<std::optional<typename MinorPolicyT_::MinorT>>;
  { policy.minor(minor) } -> std::same_as<void>;
};

static_assert(MinorPolicyT<MinorVersion<std::size_t>>);
static_assert(MinorPolicyT<NoMinorVersion>);

} // namespace Y2KaoZ::Network::Protocol::Opcode