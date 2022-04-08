#pragma once
#include "Y2KaoZ/Network/Protocol/Concepts.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/NoHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/RandomBytesHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/RandomIntHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/UnsignedHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/UuidHeaderId.hpp"
#include <cstdint>

namespace Y2KaoZ::Network::Protocol::Header {

template <class HeaderIdPolicyT_>
concept HeaderIdPolicyT = ByteSerializable<HeaderIdPolicyT_> && JsonSerializable<HeaderIdPolicyT_> &&
    requires(HeaderIdPolicyT_ policy, std::optional<typename HeaderIdPolicyT_::HeaderIdT> id) {
  typename HeaderIdPolicyT_::HeaderIdT;
  { HeaderIdPolicyT_(id) } -> std::same_as<HeaderIdPolicyT_>;
  { policy.id() } -> std::same_as<std::optional<typename HeaderIdPolicyT_::HeaderIdT>>;
  { policy.id(id) } -> std::same_as<void>;
};

static_assert(HeaderIdPolicyT<NoHeaderId>);
static_assert(HeaderIdPolicyT<RandomBytesHeaderId<0>>);
static_assert(HeaderIdPolicyT<RandomIntHeaderId<std::uint32_t>>);
static_assert(HeaderIdPolicyT<UnsignedHeaderId<std::uint32_t>>);
static_assert(HeaderIdPolicyT<UuidHeaderId>);

} // namespace Y2KaoZ::Network::Protocol::Header