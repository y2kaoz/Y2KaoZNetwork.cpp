#pragma once

#include <concepts>
#include <span>
#include <type_traits>

namespace Y2KaoZ::Network::Concepts {

template <typename C>
concept TriviallyCopyableStandardLayoutContainer = requires(C c) {
  typename C::value_type;
  std::is_standard_layout_v<typename C::value_type>;
  std::is_trivially_copyable_v<typename C::value_type>;
  { c.size() } -> std::convertible_to<std::size_t>;
  { std::data(c) } -> std::contiguous_iterator;
};

template <typename T>
concept TriviallyCopyableStandardLayoutType =
  std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T> && !TriviallyCopyableStandardLayoutContainer<T>;

} // namespace Y2KaoZ::Network::Concepts
