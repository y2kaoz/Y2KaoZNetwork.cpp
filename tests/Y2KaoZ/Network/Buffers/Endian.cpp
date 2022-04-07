#include "Y2KaoZ/Network/Buffers/Endian.hpp"
#include <catch2/catch_all.hpp>
#include <cstdint>
#include <type_traits>

TEST_CASE("Native to Endian") { // NOLINT

  using Y2KaoZ::Network::Buffers::toEndian;
  using Y2KaoZ::Network::Buffers::toNative;

  constexpr std::uint8_t one8 = 0x01;
  constexpr std::uint16_t one16 = 0x0001;
  constexpr std::uint32_t one32 = 0x00000001;
  constexpr std::uint64_t one64 = 0x0000000000000001;

  constexpr std::uint8_t inv8 = 0x01;
  constexpr std::uint16_t inv16 = 0x0100;
  constexpr std::uint32_t inv32 = 0x01000000;
  constexpr std::uint64_t inv64 = 0x0100000000000000;

  SECTION("toEndian") {
    REQUIRE(toEndian<std::endian::native>(one8) == one8);
    REQUIRE(toEndian<std::endian::native>(one16) == one16);
    REQUIRE(toEndian<std::endian::native>(one32) == one32);
    REQUIRE(toEndian<std::endian::native>(one64) == one64);

    if constexpr (std::endian::native == std::endian::little) {
      REQUIRE(toEndian<std::endian::little>(one8) == one8);
      REQUIRE(toEndian<std::endian::little>(one16) == one16);
      REQUIRE(toEndian<std::endian::little>(one32) == one32);
      REQUIRE(toEndian<std::endian::little>(one64) == one64);
      REQUIRE(toEndian<std::endian::big>(one8) == one8);
      REQUIRE(toEndian<std::endian::big>(one16) != one16);
      REQUIRE(toEndian<std::endian::big>(one32) != one32);
      REQUIRE(toEndian<std::endian::big>(one64) != one64);
      REQUIRE(toEndian<std::endian::big>(one8) == inv8);
      REQUIRE(toEndian<std::endian::big>(one16) == inv16);
      REQUIRE(toEndian<std::endian::big>(one32) == inv32);
      REQUIRE(toEndian<std::endian::big>(one64) == inv64);
    } else {
      REQUIRE(toEndian<std::endian::big>(one8) == one8);
      REQUIRE(toEndian<std::endian::big>(one16) == one16);
      REQUIRE(toEndian<std::endian::big>(one32) == one32);
      REQUIRE(toEndian<std::endian::big>(one64) == one64);
      REQUIRE(toEndian<std::endian::little>(one8) == one8);
      REQUIRE(toEndian<std::endian::little>(one16) != one16);
      REQUIRE(toEndian<std::endian::little>(one32) != one32);
      REQUIRE(toEndian<std::endian::little>(one64) != one64);
      REQUIRE(toEndian<std::endian::little>(one8) == inv8);
      REQUIRE(toEndian<std::endian::little>(one16) == inv16);
      REQUIRE(toEndian<std::endian::little>(one32) == inv32);
      REQUIRE(toEndian<std::endian::little>(one64) == inv64);
    }
  }

  SECTION("toNative") {
    REQUIRE(toNative<std::endian::native>(one8) == one8);
    REQUIRE(toNative<std::endian::native>(one16) == one16);
    REQUIRE(toNative<std::endian::native>(one32) == one32);
    REQUIRE(toNative<std::endian::native>(one64) == one64);

    if constexpr (std::endian::native == std::endian::little) {
      REQUIRE(toNative<std::endian::little>(one8) == one8);
      REQUIRE(toNative<std::endian::little>(one16) == one16);
      REQUIRE(toNative<std::endian::little>(one32) == one32);
      REQUIRE(toNative<std::endian::little>(one64) == one64);
      REQUIRE(toNative<std::endian::big>(one8) == one8);
      REQUIRE(toNative<std::endian::big>(one16) != one16);
      REQUIRE(toNative<std::endian::big>(one32) != one32);
      REQUIRE(toNative<std::endian::big>(one64) != one64);
      REQUIRE(toNative<std::endian::big>(one8) == inv8);
      REQUIRE(toNative<std::endian::big>(one16) == inv16);
      REQUIRE(toNative<std::endian::big>(one32) == inv32);
      REQUIRE(toNative<std::endian::big>(one64) == inv64);
    } else {
      REQUIRE(toNative<std::endian::big>(one8) == one8);
      REQUIRE(toNative<std::endian::big>(one16) == one16);
      REQUIRE(toNative<std::endian::big>(one32) == one32);
      REQUIRE(toNative<std::endian::big>(one64) == one64);
      REQUIRE(toNative<std::endian::little>(one8) == one8);
      REQUIRE(toNative<std::endian::little>(one16) != one16);
      REQUIRE(toNative<std::endian::little>(one32) != one32);
      REQUIRE(toNative<std::endian::little>(one64) != one64);
      REQUIRE(toNative<std::endian::little>(one8) == inv8);
      REQUIRE(toNative<std::endian::little>(one16) == inv16);
      REQUIRE(toNative<std::endian::little>(one32) == inv32);
      REQUIRE(toNative<std::endian::little>(one64) == inv64);
    }
  }

  SECTION("toEndian -> toNative") {
    REQUIRE(toNative<std::endian::little>(toEndian<std::endian::little>(one8)) == one8);
    REQUIRE(toNative<std::endian::little>(toEndian<std::endian::little>(one16)) == one16);
    REQUIRE(toNative<std::endian::little>(toEndian<std::endian::little>(one32)) == one32);
    REQUIRE(toNative<std::endian::little>(toEndian<std::endian::little>(one64)) == one64);
    REQUIRE(toNative<std::endian::big>(toEndian<std::endian::big>(one8)) == one8);
    REQUIRE(toNative<std::endian::big>(toEndian<std::endian::big>(one16)) == one16);
    REQUIRE(toNative<std::endian::big>(toEndian<std::endian::big>(one32)) == one32);
    REQUIRE(toNative<std::endian::big>(toEndian<std::endian::big>(one64)) == one64);
  }
}