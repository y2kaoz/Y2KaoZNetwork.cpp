#pragma once

#include <concepts>
#include <span>
#include <type_traits>

namespace Y2KaoZ::Network::Concepts {

template <typename T>
concept TriviallyCopyableStandardLayoutType = std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>;

template <typename C>
concept TriviallyCopyableStandardLayoutContainer = requires(C c) {
  typename C::value_type;
  TriviallyCopyableStandardLayoutType<typename C::value_type>;
  { c.size() } -> std::convertible_to<std::size_t>;
  { std::data(c) } -> std::contiguous_iterator;
};

} // namespace Y2KaoZ::Network::Concepts