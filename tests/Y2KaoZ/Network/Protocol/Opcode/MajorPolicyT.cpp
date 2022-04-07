#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode/MajorPolicyT.hpp"
#include "Y2KaoZ/Network/Buffers/JsonBufferReader.hpp"
#include <boost/json/object.hpp>
#include <catch2/catch_all.hpp>
#include <cstdint>
#include <vector>

TEST_CASE("NoMajorVersion") { // NOLINT
  using Y2KaoZ::Network::Protocol::Opcode::NoMajorVersion;
  NoMajorVersion noMajorVersion{};

  static const constexpr auto hasValue = noMajorVersion.major().has_value();

  REQUIRE(hasValue == false);
  REQUIRE_NOTHROW(noMajorVersion.major(5));

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(noMajorVersion, writer);
    REQUIRE(buffer.empty());
  }

  SECTION("byteDeserialize") {
    std::array<std::byte, 4> buffer{};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(noMajorVersion, reader);
    REQUIRE(reader.readed() == 0);
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(noMajorVersion, object);
    REQUIRE(object.empty());
  }

  SECTION("jsonDeserialize") {
    REQUIRE_NOTHROW(jsonDeserialize(
        noMajorVersion,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"major":3})"}.valueReader().object()));
    REQUIRE_NOTHROW(jsonDeserialize(
        noMajorVersion,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"Major":3})"}.valueReader().object()));
  }
}

TEST_CASE("MajorVersion") { // NOLINT
  using Y2KaoZ::Network::Protocol::Opcode::MajorVersion;
  MajorVersion<std::uint16_t> majorVersion{};

  const auto hasValue = majorVersion.major().has_value();

  REQUIRE(hasValue == true);
  REQUIRE_NOTHROW(majorVersion.major(5));
  REQUIRE(majorVersion.major().value() == 5);

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(majorVersion, writer);
    REQUIRE(buffer.size() == 2);
  }

  SECTION("byteDeserialize") {
    std::array<std::byte, 4> buffer{};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(majorVersion, reader);
    REQUIRE(reader.readed() == sizeof(std::uint16_t));
    REQUIRE(majorVersion.major().value() == 0);
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(majorVersion, object);
    REQUIRE(object.contains("major"));
  }

  SECTION("jsonDeserialize") {
    REQUIRE_NOTHROW(jsonDeserialize(
        majorVersion,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"major":3})"}.valueReader().object()));
    REQUIRE(majorVersion.major().value() == 3);
    REQUIRE_THROWS(jsonDeserialize(
        majorVersion,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"Major":3})"}.valueReader().object()));
  }
}