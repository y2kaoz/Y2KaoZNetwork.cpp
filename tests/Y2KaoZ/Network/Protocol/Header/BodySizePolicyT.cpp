#include "Y2KaoZ/Network/Buffers/JsonBufferReader.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/BodySizePolicyT/BodySize.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/BodySizePolicyT/NoBodySize.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("NoBodySize") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::NoBodySize;
  NoBodySize noBodySize{};

  static const constexpr auto hasValue = noBodySize.bodySize().has_value();

  REQUIRE(hasValue == false);
  REQUIRE_NOTHROW(noBodySize.bodySize(5));

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(noBodySize, writer);
    REQUIRE(buffer.empty());
  }

  SECTION("byteDeserialize") {
    std::array<std::byte, 4> buffer{};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(noBodySize, reader);
    REQUIRE(reader.readed() == 0);
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(noBodySize, object);
    REQUIRE(object.empty());
  }

  SECTION("jsonDeserialize") {
    REQUIRE_NOTHROW(jsonDeserialize(
        noBodySize,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"bodySize":3})"}.valueReader().object()));
    REQUIRE_NOTHROW(jsonDeserialize(
        noBodySize,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"BodySize":3})"}.valueReader().object()));
  }
}

TEST_CASE("MajorVersion") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::BodySize;
  BodySize<std::uint16_t> bodySize{};

  const auto hasValue = bodySize.bodySize().has_value();

  REQUIRE(hasValue == true);
  REQUIRE_NOTHROW(bodySize.bodySize(5));
  REQUIRE(bodySize.bodySize().value() == 5);

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(bodySize, writer);
    REQUIRE(buffer.size() == 2);
  }

  SECTION("byteDeserialize") {
    std::array<std::byte, 4> buffer{};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(bodySize, reader);
    REQUIRE(reader.readed() == sizeof(std::uint16_t));
    REQUIRE(bodySize.bodySize().value() == 0);
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(bodySize, object);
    REQUIRE(object.contains("bodySize"));
  }

  SECTION("jsonDeserialize") {
    REQUIRE_NOTHROW(jsonDeserialize(
        bodySize,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"bodySize":3})"}.valueReader().object()));
    REQUIRE(bodySize.bodySize().value() == 3);
    REQUIRE_THROWS(jsonDeserialize(
        bodySize,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"BodySize":3})"}.valueReader().object()));
  }
}