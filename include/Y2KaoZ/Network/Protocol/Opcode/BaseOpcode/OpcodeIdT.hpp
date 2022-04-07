#pragma once

#include <concepts>

namespace Y2KaoZ::Network::Protocol::Opcode {

template <typename OpcodeIdT_>
concept OpcodeIdT = (std::is_enum_v<OpcodeIdT_> && std::unsigned_integral<std::underlying_type_t<OpcodeIdT_>>) ||
                    std::unsigned_integral<OpcodeIdT_>;

} // namespace Y2KaoZ::Network::Protocol::Opcode