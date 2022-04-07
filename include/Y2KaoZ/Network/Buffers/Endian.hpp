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

constexpr inline auto toEndian(auto&& value, std::endian endian) {
  if constexpr (std::endian::native != std::endian::little) {
    if (endian == std::endian::little) {
      return toEndian<std::endian::little>(value);
    }
  }
  if constexpr (std::endian::native != std::endian::big) {
    if (endian == std::endian::big) {
      return toEndian<std::endian::big>(value);
    }
  }
  return value;
}

template <std::endian ENDIAN>
constexpr inline void toEndianInplace(auto& value) {
  if constexpr (ENDIAN == std::endian::little && std::endian::native != std::endian::little) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    boost::endian::native_to_little_inplace(value);
  }
  if constexpr (ENDIAN == std::endian::big && std::endian::native != std::endian::big) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    boost::endian::native_to_big_inplace(value);
  }
}

constexpr inline void toEndianInplace(auto& value, std::endian endian) {
  if constexpr (std::endian::native != std::endian::little) {
    if (endian == std::endian::little) {
      toEndianInplace<std::endian::little>(value);
    }
  }
  if constexpr (std::endian::native != std::endian::big) {
    if (endian == std::endian::big) {
      toEndianInplace<std::endian::big>(value);
    }
  }
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

constexpr inline auto toNative(auto&& value, std::endian endian) {
  if constexpr (std::endian::native != std::endian::little) {
    if (endian == std::endian::little) {
      return toNative<std::endian::little>(value);
    }
  }
  if constexpr (std::endian::native != std::endian::big) {
    if (endian == std::endian::big) {
      return toNative<std::endian::big>(value);
    }
  }
  return value;
}

template <std::endian ENDIAN>
constexpr inline void toNativeInplace(auto& value) {
  if constexpr (ENDIAN == std::endian::little && std::endian::native != std::endian::little) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    boost::endian::little_to_native_inplace(value);
  }
  if constexpr (ENDIAN == std::endian::big && std::endian::native != std::endian::big) {
    // TODO(y2kaoz) update to std::byteswap(value) on C++23
    boost::endian::big_to_native_inplace(value);
  }
}

constexpr inline void toNativeInplace(auto& value, std::endian endian) {
  if constexpr (std::endian::native != std::endian::little) {
    if (endian == std::endian::little) {
      toNativeInplace<std::endian::little>(value);
    }
  }
  if constexpr (std::endian::native != std::endian::big) {
    if (endian == std::endian::big) {
      toNativeInplace<std::endian::big>(value);
    }
  }
}

} // namespace Y2KaoZ::Network::Buffers