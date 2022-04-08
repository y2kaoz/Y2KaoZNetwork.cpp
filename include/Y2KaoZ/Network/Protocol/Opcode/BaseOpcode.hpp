#pragma once

#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode/MajorPolicyT.hpp"
#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode/MinorPolicyT.hpp"
#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode/OpcodeIdT.hpp"
#include <cstdint>

namespace Y2KaoZ::Network::Protocol::Opcode {

template <OpcodeIdT OpcodeIdT_, MajorPolicyT MajorPolicy_, MinorPolicyT MinorPolicy_>
Y2KAOZNETWORK_EXPORT class BaseOpcode {
public:
  using OpcodeIdT = OpcodeIdT_;

  using MajorPolicy = MajorPolicy_;
  using MajorT = typename MajorPolicy::MajorT;

  using MinorPolicy = MinorPolicy_;
  using MinorT = typename MinorPolicy::MinorT;

  static constexpr const OpcodeIdT REJECTED = static_cast<OpcodeIdT>(-1);

  constexpr BaseOpcode() = default;
  auto operator==(const BaseOpcode&) const -> bool = default;

  constexpr explicit BaseOpcode(
      const OpcodeIdT& id,
      const std::optional<MajorT>& major = {},
      const std::optional<MinorT>& minor = {})
      : id_{id}
      , majorPolicy_(major)
      , minorPolicy_(minor) {
  }

  [[nodiscard]] constexpr auto id() const noexcept -> const OpcodeIdT& {
    return id_;
  }

  constexpr void id(const OpcodeIdT& id) {
    id_ = id;
  }

  [[nodiscard]] constexpr auto major() const noexcept -> std::optional<MajorT> {
    return majorPolicy_.major();
  }

  constexpr void major(const std::optional<MajorT>& major) {
    majorPolicy_.major(major);
  }

  [[nodiscard]] constexpr auto minor() const noexcept -> std::optional<MinorT> {
    return minorPolicy_.minor();
  }

  constexpr void minor(const std::optional<MinorT>& minor) {
    minorPolicy_.minor(minor);
  }

private:
  OpcodeIdT id_{};
  [[no_unique_address]] MajorPolicy_ majorPolicy_{};
  [[no_unique_address]] MinorPolicy_ minorPolicy_{};
};

static_assert(
    sizeof(BaseOpcode<std::uint16_t, Opcode::NoMajorVersion, Opcode::NoMinorVersion>) ==
    sizeof(BaseOpcode<std::uint16_t, Opcode::NoMajorVersion, Opcode::NoMinorVersion>::OpcodeIdT));

template <OpcodeIdT OpcodeIdT_, MajorPolicyT MajorPolicy_, MinorPolicyT MinorPolicy_>
Y2KAOZNETWORK_EXPORT void byteSerialize(
    const BaseOpcode<OpcodeIdT_, MajorPolicy_, MinorPolicy_>& in,
    Buffers::VectBufferWriter& out,
    std::endian endian = std::endian::native) {
  out.write(in.id(), endian);
  byteSerialize(MajorPolicy_{in.major()}, out, endian);
  byteSerialize(MinorPolicy_{in.minor()}, out, endian);
}

template <OpcodeIdT OpcodeIdT_, MajorPolicyT MajorPolicy_, MinorPolicyT MinorPolicy_>
Y2KAOZNETWORK_EXPORT void byteDeserialize(
    BaseOpcode<OpcodeIdT_, MajorPolicy_, MinorPolicy_>& out,
    Buffers::SpanBufferReader& in,
    std::endian endian = std::endian::native) {
  auto reader = in;

  OpcodeIdT_ id{};
  MajorPolicy_ majorPolicy{};
  MinorPolicy_ minorPolicy{};

  reader.read(id, endian);
  byteDeserialize(majorPolicy, reader, endian);
  byteDeserialize(minorPolicy, reader, endian);

  out.id(id);
  out.major(majorPolicy.major());
  out.minor(minorPolicy.minor());

  in.seek(reader.readed());
}

template <OpcodeIdT OpcodeIdT_, MajorPolicyT MajorPolicy_, MinorPolicyT MinorPolicy_>
Y2KAOZNETWORK_EXPORT inline void jsonSerialize(
    const BaseOpcode<OpcodeIdT_, MajorPolicy_, MinorPolicy_>& in,
    boost::json::object& out) {
  boost::json::object opcode{
      {"id", gsl::narrow<std::uint64_t>(in.id())},
  };
  jsonSerialize(MajorPolicy_{in.major()}, opcode);
  jsonSerialize(MinorPolicy_{in.minor()}, opcode);
  out["opcode"] = opcode;
}

template <OpcodeIdT OpcodeIdT_, MajorPolicyT MajorPolicy_, MinorPolicyT MinorPolicy_>
Y2KAOZNETWORK_EXPORT inline void jsonDeserialize(
    BaseOpcode<OpcodeIdT_, MajorPolicy_, MinorPolicy_>& out,
    const boost::json::object& in) {
  OpcodeIdT_ id{};
  MajorPolicy_ majorPolicy{};
  MinorPolicy_ minorPolicy{};

  auto opcode = Buffers::JsonValueReader{in, "opcode"}.object();

  id = gsl::narrow<OpcodeIdT_>(Buffers::JsonValueReader{opcode, "id"}.uint64());
  jsonDeserialize(majorPolicy, opcode);
  jsonDeserialize(minorPolicy, opcode);

  out.id(id);
  out.major(majorPolicy.major());
  out.minor(minorPolicy.minor());
}

template <OpcodeIdT OpcodeIdT_, MajorPolicyT MajorPolicy_, MinorPolicyT MinorPolicy_>
Y2KAOZNETWORK_EXPORT inline auto operator<<(
    std::ostream& out,
    const BaseOpcode<OpcodeIdT_, MajorPolicy_, MinorPolicy_>& in) -> std::ostream& {
  boost::json::object buffer{};
  jsonSerialize(in, buffer);
  out << boost::json::serialize(buffer);
  return out;
}

} // namespace Y2KaoZ::Network::Protocol::Opcode