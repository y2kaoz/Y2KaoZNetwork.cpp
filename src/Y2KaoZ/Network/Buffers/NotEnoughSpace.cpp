#include "Y2KaoZ/Network/Buffers/NotEnoughSpace.hpp"

namespace Y2KaoZ::Network::Buffers {

NotEnoughSpace::NotEnoughSpace(std::size_t difference)
  : std::out_of_range("Not enough space in the buffer for a successfull write.")
  , difference_(difference) {
}

auto NotEnoughSpace::difference() const noexcept -> std::size_t {
  return difference_;
}

} // namespace Y2KaoZ::Network::Buffers