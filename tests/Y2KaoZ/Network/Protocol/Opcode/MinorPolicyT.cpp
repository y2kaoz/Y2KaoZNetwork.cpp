#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode/MinorPolicyT.hpp"
#include "Y2KaoZ/Network/Buffers/JsonBufferReader.hpp"
#include <boost/json/object.hpp>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <vector>

TEST_CASE("NoMinorVersion") { // NOLINT
  using Y2KaoZ::Network::Protocol::Opcode::NoMinorVersion;
  NoMinorVersion noMinorVersion{};

  static const constexpr auto hasValue = noMinorVersion.minor().has_value();

  REQUIRE(hasValue == false);
  REQUIRE_NOTHROW(noMinorVersion.minor(5));

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(noMinorVersion, writer);
    REQUIRE(buffer.empty());
  }

  SECTION("byteDeserialize") {
    std::array<std::byte, 4> buffer{};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(noMinorVersion, reader);
    REQUIRE(reader.readed() == 0);
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(noMinorVersion, object);
    REQUIRE(object.empty());
  }

  SECTION("jsonDeserialize") {
    REQUIRE_NOTHROW(jsonDeserialize(
        noMinorVersion,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"minor":3})"}.valueReader().object()));
    REQUIRE_NOTHROW(jsonDeserialize(
        noMinorVersion,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"Minor":3})"}.valueReader().object()));
  }
}

TEST_CASE("MinorVersion") { // NOLINT
  using Y2KaoZ::Network::Protocol::Opcode::MinorVersion;
  MinorVersion<std::uint16_t> minorVersion{};

  const auto hasValue = minorVersion.minor().has_value();

  REQUIRE(hasValue == true);
  REQUIRE_NOTHROW(minorVersion.minor(5));
  REQUIRE(minorVersion.minor().value() == 5);

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(minorVersion, writer);
    REQUIRE(buffer.size() == 2);
  }

  SECTION("byteDeserialize") {
    std::array<std::byte, 4> buffer{};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(minorVersion, reader);
    REQUIRE(reader.readed() == sizeof(std::uint16_t));
    REQUIRE(minorVersion.minor().value() == 0);
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(minorVersion, object);
    REQUIRE(object.contains("minor"));
  }

  SECTION("jsonDeserialize") {
    REQUIRE_NOTHROW(jsonDeserialize(
        minorVersion,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"minor":3})"}.valueReader().object()));
    REQUIRE(minorVersion.minor().value() == 3);
    REQUIRE_THROWS(jsonDeserialize(
        minorVersion,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"Minor":3})"}.valueReader().object()));
  }
}