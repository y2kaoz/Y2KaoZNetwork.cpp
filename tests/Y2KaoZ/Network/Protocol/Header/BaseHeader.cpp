#include "Y2KaoZ/Network/Protocol/Header/BaseHeader.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/BodySizePolicyT/BodySize.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/BodySizePolicyT/NoBodySize.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/NoHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/UnsignedHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Header/BaseHeader/HeaderIdPolicyT/UuidHeaderId.hpp"
#include "Y2KaoZ/Network/Protocol/Opcode.hpp"
#include <boost/uuid/uuid.hpp>
#include <catch2/catch_all.hpp>
#include <cstdint>
#include <iostream>

TEST_CASE("Only Opcode") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::BaseHeader;
  using Y2KaoZ::Network::Protocol::Header::NoBodySize;
  using Y2KaoZ::Network::Protocol::Header::NoHeaderId;
  using Y2KaoZ::Network::Protocol::Opcode::MediumOpcode;

  using HeaderT = BaseHeader<MediumOpcode, NoHeaderId, NoBodySize>;

  const auto opcode = MediumOpcode{1, 2, 3};

  HeaderT header{opcode};
  REQUIRE(header.opcode() == opcode);
  REQUIRE(!header.id().has_value());
  REQUIRE(!header.bodySize().has_value());

  SECTION("byteSerialize/byteDeserialize Native") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(header, writer);
    REQUIRE(buffer.size() == sizeof(opcode));

    HeaderT deserialized{};
    REQUIRE(deserialized.opcode() == MediumOpcode{0, 0, 0});
    REQUIRE(!deserialized.id().has_value());
    REQUIRE(!deserialized.bodySize().has_value());

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader);
    REQUIRE(reader.readed() == sizeof(opcode));
    REQUIRE(deserialized.opcode() == opcode);
    REQUIRE(!deserialized.id().has_value());
    REQUIRE(!deserialized.bodySize().has_value());
  }

  SECTION("byteSerialize/byteDeserialize Endian") {
    std::endian endian = std::endian::native == std::endian::little ? std::endian::big : std::endian::little;

    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(header, writer, endian);
    REQUIRE(buffer.size() == sizeof(opcode));

    HeaderT deserialized{};
    REQUIRE(deserialized.opcode() == MediumOpcode{0, 0, 0});
    REQUIRE(!deserialized.id().has_value());
    REQUIRE(!deserialized.bodySize().has_value());

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader, endian);
    REQUIRE(reader.readed() == sizeof(opcode));
    REQUIRE(deserialized.opcode() == opcode);
    REQUIRE(!deserialized.id().has_value());
    REQUIRE(!deserialized.bodySize().has_value());
  }

  SECTION("jsonSerialize/jsonDeserialize") {
    boost::json::object object{};
    jsonSerialize(header, object);
    REQUIRE(object.size() == 1);

    HeaderT deserialized{};
    REQUIRE(deserialized.opcode() == MediumOpcode{0, 0, 0});
    REQUIRE(!deserialized.id().has_value());
    REQUIRE(!deserialized.bodySize().has_value());

    jsonDeserialize(deserialized, object);
    REQUIRE(deserialized.opcode() == opcode);
    REQUIRE(!deserialized.id().has_value());
    REQUIRE(!deserialized.bodySize().has_value());
  }
}

TEST_CASE("Opcode and Id") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::BaseHeader;
  using Y2KaoZ::Network::Protocol::Header::NoBodySize;
  using Y2KaoZ::Network::Protocol::Header::UnsignedHeaderId;
  using Y2KaoZ::Network::Protocol::Opcode::MediumOpcode;

  using HeaderT = BaseHeader<MediumOpcode, UnsignedHeaderId<std::uint32_t>, NoBodySize>;

  const auto opcode = MediumOpcode{1, 2, 3};

  HeaderT header{opcode, 4};
  REQUIRE(header.opcode() == opcode);
  REQUIRE((header.id().has_value() && header.id().value() == 4));
  REQUIRE(!header.bodySize().has_value());

  SECTION("byteSerialize/byteDeserialize Native") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(header, writer);
    REQUIRE(buffer.size() == sizeof(opcode) + sizeof(std::uint32_t));

    HeaderT deserialized{};
    REQUIRE(deserialized.opcode() == MediumOpcode{0, 0, 0});
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 0));
    REQUIRE(!deserialized.bodySize().has_value());

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader);
    REQUIRE(reader.readed() == sizeof(opcode) + sizeof(std::uint32_t));
    REQUIRE(deserialized.opcode() == opcode);
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 4));
    REQUIRE(!deserialized.bodySize().has_value());
  }

  SECTION("byteSerialize/byteDeserialize Endian") {
    std::endian endian = std::endian::native == std::endian::little ? std::endian::big : std::endian::little;

    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(header, writer, endian);
    REQUIRE(buffer.size() == sizeof(opcode) + sizeof(std::uint32_t));

    HeaderT deserialized{};
    REQUIRE(deserialized.opcode() == MediumOpcode{0, 0, 0});
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 0));
    REQUIRE(!deserialized.bodySize().has_value());

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader, endian);
    REQUIRE(reader.readed() == sizeof(opcode) + sizeof(std::uint32_t));
    REQUIRE(deserialized.opcode() == opcode);
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 4));
    REQUIRE(!deserialized.bodySize().has_value());
  }

  SECTION("jsonSerialize/jsonDeserialize") {
    boost::json::object object{};
    jsonSerialize(header, object);
    REQUIRE(object.size() == 1);

    HeaderT deserialized{};
    REQUIRE(deserialized.opcode() == MediumOpcode{0, 0, 0});
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 0));
    REQUIRE(!deserialized.bodySize().has_value());

    jsonDeserialize(deserialized, object);
    REQUIRE(deserialized.opcode() == opcode);
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 4));
    REQUIRE(!deserialized.bodySize().has_value());
  }
}

TEST_CASE("Opcode, Id and Size") { // NOLINT
  using Y2KaoZ::Network::Protocol::Header::BaseHeader;
  using Y2KaoZ::Network::Protocol::Header::BodySize;
  using Y2KaoZ::Network::Protocol::Header::UnsignedHeaderId;
  using Y2KaoZ::Network::Protocol::Opcode::MediumOpcode;

  using HeaderT = BaseHeader<MediumOpcode, UnsignedHeaderId<std::uint32_t>, BodySize<std::uint16_t>>;

  const auto opcode = MediumOpcode{1, 2, 3};

  HeaderT header{opcode, 4, sizeof(std::uint64_t)};
  REQUIRE(header.opcode() == opcode);
  REQUIRE((header.id().has_value() && header.id().value() == 4));
  REQUIRE((header.bodySize().has_value() && header.bodySize().value() == sizeof(std::uint64_t)));

  SECTION("byteSerialize/byteDeserialize Native") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(header, writer);
    REQUIRE(buffer.size() == sizeof(opcode) + sizeof(std::uint32_t) + sizeof(std::uint16_t));

    HeaderT deserialized{};
    REQUIRE(deserialized.opcode() == MediumOpcode{0, 0, 0});
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 0));
    REQUIRE((deserialized.bodySize().has_value() && deserialized.bodySize().value() == 0));

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader);
    REQUIRE(reader.readed() == sizeof(opcode) + sizeof(std::uint32_t) + sizeof(std::uint16_t));
    REQUIRE(deserialized.opcode() == opcode);
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 4));
    REQUIRE((deserialized.bodySize().has_value() && deserialized.bodySize().value() == sizeof(std::uint64_t)));
  }

  SECTION("byteSerialize/byteDeserialize Endian") {
    std::endian endian = std::endian::native == std::endian::little ? std::endian::big : std::endian::little;

    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(header, writer, endian);
    REQUIRE(buffer.size() == sizeof(opcode) + sizeof(std::uint32_t) + sizeof(std::uint16_t));

    HeaderT deserialized{};
    REQUIRE(deserialized.opcode() == MediumOpcode{0, 0, 0});
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 0));
    REQUIRE((deserialized.bodySize().has_value() && deserialized.bodySize().value() == 0));

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader, endian);
    REQUIRE(reader.readed() == sizeof(opcode) + sizeof(std::uint32_t) + sizeof(std::uint16_t));
    REQUIRE(deserialized.opcode() == opcode);
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 4));
    REQUIRE((deserialized.bodySize().has_value() && deserialized.bodySize().value() == sizeof(std::uint64_t)));
  }

  SECTION("jsonSerialize/jsonDeserialize") {
    boost::json::object object{};
    jsonSerialize(header, object);
    REQUIRE(object.size() == 1);

    HeaderT deserialized{};
    REQUIRE(deserialized.opcode() == MediumOpcode{0, 0, 0});
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 0));
    REQUIRE((deserialized.bodySize().has_value() && deserialized.bodySize().value() == 0));

    jsonDeserialize(deserialized, object);
    REQUIRE(deserialized.opcode() == opcode);
    REQUIRE((deserialized.id().has_value() && deserialized.id().value() == 4));
    REQUIRE((deserialized.bodySize().has_value() && deserialized.bodySize().value() == sizeof(std::uint64_t)));
  }
}