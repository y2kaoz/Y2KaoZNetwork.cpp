#include "Y2KaoZ/Network/Buffers/NotEnoughData.hpp"

namespace Y2KaoZ::Network::Buffers {

NotEnoughData::NotEnoughData(std::size_t difference)
    : std::out_of_range("Not enough data in the buffer for a successfull read.")
    , difference_(difference) {
}
auto NotEnoughData::difference() const noexcept -> std::size_t {
  return difference_;
}

} // namespace Y2KaoZ::Network::Buffers