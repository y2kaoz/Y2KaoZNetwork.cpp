#pragma once

#include "Y2KaoZ/Network/Visibility.hpp"
#include <bit>
#include <boost/endian/conversion.hpp>
#include <utility>

namespace Y2KaoZ::Network::Buffers {

template <std::endian ENDIAN>
constexpr inline auto toEndian(auto&& value) {
  if constexpr (ENDIAN == std::endian::little && std::endian::native != std::endian::little) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    return boost::endian::native_to_little(value);
  }
  if constexpr (ENDIAN == std::endian::big && std::endian::native != std::endian::big) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    return boost::endian::native_to_big(value);
  }
  return value;
}

template <std::endian ENDIAN>
constexpr inline auto toEndianInplace(auto&& value) {
  if constexpr (ENDIAN == std::endian::little && std::endian::native != std::endian::little) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    return boost::endian::native_to_little_inplace(value);
  }
  if constexpr (ENDIAN == std::endian::big && std::endian::native != std::endian::big) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    return boost::endian::native_to_big_inplace(value);
  }
  return value;
}

template <std::endian ENDIAN>
constexpr inline auto toNative(auto&& value) {
  if constexpr (ENDIAN == std::endian::little && std::endian::native != std::endian::little) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    return boost::endian::little_to_native(value);
  }
  if constexpr (ENDIAN == std::endian::big && std::endian::native != std::endian::big) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    return boost::endian::big_to_native(value);
  }
  return value;
}

template <std::endian ENDIAN>
constexpr inline auto toNativeInplace(auto&& value) {
  if constexpr (ENDIAN == std::endian::little && std::endian::native != std::endian::little) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    return boost::endian::little_to_native_inplace(value);
  }
  if constexpr (ENDIAN == std::endian::big && std::endian::native != std::endian::big) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    return boost::endian::big_to_native_inplace(value);
  }
  return value;
}

} // namespace Y2KaoZ::Network::Buffers