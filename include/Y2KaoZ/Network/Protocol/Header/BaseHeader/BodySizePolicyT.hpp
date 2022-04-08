#pragma once

#include "Y2KaoZ/Network/Protocol/Concepts.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/BodySizePolicyT/BodySize.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/BodySizePolicyT/NoBodySize.hpp"

namespace Y2KaoZ::Network::Protocol::Header {

template <class BodySizePolicyT_>
concept BodySizePolicyT = ByteSerializable<BodySizePolicyT_> && JsonSerializable<BodySizePolicyT_> &&
    requires(BodySizePolicyT_ policy, std::optional<typename BodySizePolicyT_::BodySizeT> bodySize) {
  std::unsigned_integral<typename BodySizePolicyT_::BodySizeT>;
  { BodySizePolicyT_(bodySize) } -> std::same_as<BodySizePolicyT_>;
  { policy.bodySize() } -> std::same_as<std::optional<typename BodySizePolicyT_::BodySizeT>>;
  { policy.bodySize(bodySize) } -> std::same_as<void>;
};

static_assert(BodySizePolicyT<BodySize<std::size_t>>);
static_assert(BodySizePolicyT<NoBodySize>);

} // namespace Y2KaoZ::Network::Protocol::Header