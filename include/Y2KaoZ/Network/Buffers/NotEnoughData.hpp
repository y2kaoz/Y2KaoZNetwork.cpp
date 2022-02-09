#pragma once

#include "Y2KaoZ/Network/Visibility.hpp"
#include <stdexcept>

namespace Y2KaoZ::Network::Buffers {

class Y2KAOZNETWORK_EXPORT NotEnoughData : public std::out_of_range {
  std::size_t difference_;

public:
  explicit NotEnoughData(std::size_t difference);
  [[nodiscard]] auto difference() const noexcept -> std::size_t;
};

} // namespace Y2KaoZ::Network::Buffers