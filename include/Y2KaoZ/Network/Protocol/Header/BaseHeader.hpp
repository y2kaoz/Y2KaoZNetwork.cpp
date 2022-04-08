#pragma once

#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/BodySizePolicyT.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT.hpp"
#include "Y2KaoZ/Network/Protocol/Opcode/AnyOpcodeT.hpp"

namespace Y2KaoZ::Network::Protocol::Header {

template <Opcode::AnyOpcodeT AnyOpcode_, HeaderIdPolicyT HeaderIdPolicy_, BodySizePolicyT BodySizePolicy_>
Y2KAOZNETWORK_EXPORT class BaseHeader {
public:
  using OpcodeT = AnyOpcode_;

  using HeaderIdPolicy = HeaderIdPolicy_;
  using HeaderIdT = typename HeaderIdPolicy::HeaderIdT;

  using BodySizePolicy = BodySizePolicy_;
  using BodySizeT = typename BodySizePolicy::BodySizeT;

  static constexpr const OpcodeT REJECTED = OpcodeT{OpcodeT::REJECTED, 0, 0};

  constexpr BaseHeader() = default;

  constexpr explicit BaseHeader(
      const OpcodeT& opcode,
      const std::optional<HeaderIdT>& id = {},
      const std::optional<BodySizeT>& bodySize = {})
      : opcode_{opcode}
      , headerIdPolicy_{id}
      , bodySizePolicy_{bodySize} {
  }

  [[nodiscard]] constexpr auto opcode() const noexcept -> const OpcodeT& {
    return opcode_;
  }
  constexpr void opcode(const OpcodeT& opcode) {
    opcode_ = opcode;
  }

  [[nodiscard]] constexpr auto id() const noexcept -> std::optional<HeaderIdT> {
    return headerIdPolicy_.id();
  }
  constexpr void id(const std::optional<HeaderIdT>& id) {
    headerIdPolicy_.id(id);
  }

  [[nodiscard]] constexpr auto bodySize() const noexcept -> std::optional<BodySizeT> {
    return bodySizePolicy_.bodySize();
  }
  constexpr void bodySize(const std::optional<BodySizeT>& bodySize) {
    bodySizePolicy_.bodySize(bodySize);
  }

private:
  OpcodeT opcode_{};
  [[no_unique_address]] HeaderIdPolicy headerIdPolicy_{};
  [[no_unique_address]] BodySizePolicy bodySizePolicy_{};
};

static_assert(
    sizeof(BaseHeader<
           Opcode::BaseOpcode<std::uint16_t, Opcode::NoMajorVersion, Opcode::NoMinorVersion>,
           NoHeaderId,
           NoBodySize>) ==
    sizeof(BaseHeader<
           Opcode::BaseOpcode<std::uint16_t, Opcode::NoMajorVersion, Opcode::NoMinorVersion>,
           NoHeaderId,
           NoBodySize>::OpcodeT));

template <Opcode::AnyOpcodeT AnyOpcode_, HeaderIdPolicyT HeaderIdPolicy_, BodySizePolicyT BodySizePolicy_>
Y2KAOZNETWORK_EXPORT void byteSerialize(
    const BaseHeader<AnyOpcode_, HeaderIdPolicy_, BodySizePolicy_>& in,
    Buffers::VectBufferWriter& out,
    std::endian endian = std::endian::native) {
  byteSerialize(in.opcode(), out, endian);
  byteSerialize(HeaderIdPolicy_{in.id()}, out, endian);
  byteSerialize(BodySizePolicy_{in.bodySize()}, out, endian);
}

template <Opcode::AnyOpcodeT AnyOpcode_, HeaderIdPolicyT HeaderIdPolicy_, BodySizePolicyT BodySizePolicy_>
Y2KAOZNETWORK_EXPORT void byteDeserialize(
    BaseHeader<AnyOpcode_, HeaderIdPolicy_, BodySizePolicy_>& out,
    Buffers::SpanBufferReader& in,
    std::endian endian = std::endian::native) {
  auto reader = in;

  AnyOpcode_ opcode{};
  HeaderIdPolicy_ headerIdPolicy{};
  BodySizePolicy_ bodySizePolicy{};

  byteDeserialize(opcode, reader, endian);
  byteDeserialize(headerIdPolicy, reader, endian);
  byteDeserialize(bodySizePolicy, reader, endian);

  out.opcode(opcode);
  out.id(headerIdPolicy.id());
  out.bodySize(bodySizePolicy.bodySize());

  in.seek(reader.readed());
}

template <Opcode::AnyOpcodeT AnyOpcode_, HeaderIdPolicyT HeaderIdPolicy_, BodySizePolicyT BodySizePolicy_>
Y2KAOZNETWORK_EXPORT inline void jsonSerialize(
    const BaseHeader<AnyOpcode_, HeaderIdPolicy_, BodySizePolicy_>& in,
    boost::json::object& out) {
  boost::json::object header{};
  jsonSerialize(AnyOpcode_{in.opcode()}, header);
  jsonSerialize(HeaderIdPolicy_{in.id()}, header);
  jsonSerialize(BodySizePolicy_{in.bodySize()}, header);
  out["header"] = header;
}

template <Opcode::AnyOpcodeT AnyOpcode_, HeaderIdPolicyT HeaderIdPolicy_, BodySizePolicyT BodySizePolicy_>
Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(
    BaseHeader<AnyOpcode_, HeaderIdPolicy_, BodySizePolicy_>& out,
    const boost::json::object& in) {

  AnyOpcode_ opcode{};
  HeaderIdPolicy_ headerIdPolicy{};
  BodySizePolicy_ bodySizePolicy{};

  auto header = Buffers::JsonValueReader{in, "header"}.object();

  jsonDeserialize(opcode, header);
  jsonDeserialize(headerIdPolicy, header);
  jsonDeserialize(bodySizePolicy, header);

  out.opcode(opcode);
  out.id(headerIdPolicy.id());
  out.bodySize(bodySizePolicy.bodySize());
}

} // namespace Y2KaoZ::Network::Protocol::Header