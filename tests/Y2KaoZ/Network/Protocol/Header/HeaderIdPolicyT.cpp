#include "Y2KaoZ/Network/Buffers/JsonBufferReader.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/NoHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/RandomBytesHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/RandomIntHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/UnsignedHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/UuidHeaderId.hpp"
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>

TEST_CASE("NoHeaderId") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::NoHeaderId;
  NoHeaderId noHeaderId{};

  static const constexpr auto hasValue = noHeaderId.id().has_value();

  REQUIRE(hasValue == false);
  REQUIRE_NOTHROW(noHeaderId.id(nullptr));

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(noHeaderId, writer);
    REQUIRE(buffer.empty());
  }

  SECTION("byteDeserialize") {
    std::array<std::byte, 4> buffer{};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(noHeaderId, reader);
    REQUIRE(reader.readed() == 0);
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(noHeaderId, object);
    REQUIRE(object.empty());
  }

  SECTION("jsonDeserialize") {
    REQUIRE_NOTHROW(
        jsonDeserialize(noHeaderId, Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"id":3})"}.valueReader().object()));
    REQUIRE_NOTHROW(
        jsonDeserialize(noHeaderId, Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"Id":3})"}.valueReader().object()));
  }
}

TEST_CASE("UnsignedHeaderId") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::UnsignedHeaderId;
  UnsignedHeaderId<std::uint16_t> id{};

  const auto hasValue = id.id().has_value();

  REQUIRE(hasValue == true);
  REQUIRE_NOTHROW(id.id(5));
  REQUIRE(id.id().value() == 5);

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(id, writer);
    REQUIRE(buffer.size() == 2);
  }

  SECTION("byteDeserialize") {
    std::array<std::byte, 4> buffer{};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(id, reader);
    REQUIRE(reader.readed() == sizeof(std::uint16_t));
    REQUIRE(id.id().value() == 0);
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(id, object);
    REQUIRE(object.contains("id"));
  }

  SECTION("jsonDeserialize") {
    REQUIRE_NOTHROW(
        jsonDeserialize(id, Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"id":3})"}.valueReader().object()));
    REQUIRE(id.id().value() == 3);
    REQUIRE_THROWS(
        jsonDeserialize(id, Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"Id":3})"}.valueReader().object()));
  }
}

TEST_CASE("RandomBytesHeaderId") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::RandomBytesHeaderId;
  RandomBytesHeaderId<sizeof(std::size_t)> id{};

  const auto hasValue = id.id().has_value();
  REQUIRE(hasValue == true);

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(id, writer);
    REQUIRE(buffer.size() == sizeof(std::size_t));
  }

  SECTION("byteDeserialize") {
    std::vector<std::byte> buffer{4};
    {
      Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
      REQUIRE_THROWS(byteDeserialize(id, reader));
    }
    {
      buffer.resize(sizeof(std::size_t));
      Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
      REQUIRE_NOTHROW(byteDeserialize(id, reader));
      REQUIRE(reader.readed() == sizeof(std::size_t));
    }
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(id, object);
    REQUIRE(object.contains("id"));
  }

  SECTION("jsonDeserialize") {
    const std::array<unsigned char, 8> expected{{0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef}};
    REQUIRE_NOTHROW(jsonDeserialize(
        id,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"id":"deadbeefdeadbeef"})"}.valueReader().object()));
    REQUIRE(id.id().value() == expected);
    REQUIRE_THROWS(jsonDeserialize(
        id,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"Id":"deadbeefdeadbeef"})"}.valueReader().object()));
  }
}

TEST_CASE("RandomIntHeaderId") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::RandomIntHeaderId;
  RandomIntHeaderId<std::uint32_t> id{};

  const auto hasValue = id.id().has_value();
  REQUIRE(hasValue == true);

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(id, writer);
    REQUIRE(buffer.size() == sizeof(std::uint32_t));
  }

  SECTION("byteDeserialize") {
    std::vector<std::byte> buffer{sizeof(std::uint32_t)};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    REQUIRE_NOTHROW(byteDeserialize(id, reader));
    REQUIRE(reader.readed() == sizeof(std::uint32_t));
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(id, object);
    REQUIRE(object.contains("id"));
  }

  SECTION("jsonDeserialize") {
    REQUIRE_NOTHROW(
        jsonDeserialize(id, Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"id":4})"}.valueReader().object()));
    REQUIRE(id.id().value() == 4);
    REQUIRE_THROWS(
        jsonDeserialize(id, Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"Id":4})"}.valueReader().object()));
  }
}

TEST_CASE("UuidHeaderId") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::UuidHeaderId;
  UuidHeaderId id{};

  const auto hasValue = id.id().has_value();

  REQUIRE(hasValue == true);
  REQUIRE(!id.id().value().is_nil());

  SECTION("byteSerialize") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(id, writer);
    REQUIRE(buffer.size() == sizeof(boost::uuids::uuid));
  }

  SECTION("byteDeserialize") {
    std::vector<std::byte> buffer{sizeof(boost::uuids::uuid)};
    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    REQUIRE_NOTHROW(byteDeserialize(id, reader));
    REQUIRE(reader.readed() == sizeof(boost::uuids::uuid));
  }

  SECTION("jsonSerialize") {
    boost::json::object object{};
    jsonSerialize(id, object);
    REQUIRE(object.contains("id"));
  }

  SECTION("jsonDeserialize") {
    boost::uuids::string_generator gen;
    const auto expected = gen("aa435575-f552-4ea7-80c7-62223273cdd9");
    REQUIRE_NOTHROW(jsonDeserialize(
        id,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"id":"aa435575-f552-4ea7-80c7-62223273cdd9"})"}
            .valueReader()
            .object()));
    REQUIRE(id.id().value() == expected);
    REQUIRE_THROWS(jsonDeserialize(
        id,
        Y2KaoZ::Network::Buffers::JsonBufferReader{R"({"Id":"aa435575-f552-4ea7-80c7-62223273cdd9"})"}
            .valueReader()
            .object()));
  }
}
