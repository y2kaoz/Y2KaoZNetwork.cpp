#include "Y2KaoZ/Network/Protocol/Opcode/BaseOpcode.hpp"
#include <boost/json/object.hpp>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <type_traits>

TEST_CASE("Only Id") { // NOLINT
  using Y2KaoZ::Network::Protocol::Opcode::BaseOpcode;
  using Y2KaoZ::Network::Protocol::Opcode::NoMajorVersion;
  using Y2KaoZ::Network::Protocol::Opcode::NoMinorVersion;
  using OpcodeT = BaseOpcode<std::uint16_t, NoMajorVersion, NoMinorVersion>;

  const OpcodeT opcode{1, 2, 3};
  REQUIRE(opcode.id() == 1);
  REQUIRE(!opcode.major().has_value());
  REQUIRE(!opcode.minor().has_value());

  SECTION("byteSerialize/byteDeserialize Native") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(opcode, writer);
    REQUIRE(buffer.size() == sizeof(std::uint16_t));

    OpcodeT deserialized{3, 2, 1};
    REQUIRE(deserialized.id() == 3);
    REQUIRE(!deserialized.major().has_value());
    REQUIRE(!deserialized.minor().has_value());

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader);
    REQUIRE(reader.readed() == sizeof(std::uint16_t));
    REQUIRE(deserialized.id() == 1);
    REQUIRE(!deserialized.major().has_value());
    REQUIRE(!deserialized.minor().has_value());
  }

  SECTION("byteSerialize/byteDeserialize Endian") {
    std::endian endian = std::endian::native == std::endian::little ? std::endian::big : std::endian::little;
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(opcode, writer, endian);
    REQUIRE(buffer.size() == sizeof(std::uint16_t));

    OpcodeT deserialized{3, 2, 1};
    REQUIRE(deserialized.id() == 3);
    REQUIRE(!deserialized.major().has_value());
    REQUIRE(!deserialized.minor().has_value());

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader, endian);
    REQUIRE(reader.readed() == sizeof(std::uint16_t));
    REQUIRE(deserialized.id() == 1);
    REQUIRE(!deserialized.major().has_value());
    REQUIRE(!deserialized.minor().has_value());
  }

  SECTION("jsonSerialize/jsonDeserialize") {
    boost::json::object object{};
    jsonSerialize(opcode, object);
    REQUIRE(object.size() == 1);

    OpcodeT deserialized{3, 2, 1};
    REQUIRE(deserialized.id() == 3);
    REQUIRE(!deserialized.major().has_value());
    REQUIRE(!deserialized.minor().has_value());

    jsonDeserialize(deserialized, object);
    REQUIRE(deserialized.id() == 1);
    REQUIRE(!deserialized.major().has_value());
    REQUIRE(!deserialized.minor().has_value());
  }
}

TEST_CASE("Id And Major") { // NOLINT
  using Y2KaoZ::Network::Protocol::Opcode::BaseOpcode;
  using Y2KaoZ::Network::Protocol::Opcode::MajorVersion;
  using Y2KaoZ::Network::Protocol::Opcode::NoMinorVersion;
  using OpcodeT = BaseOpcode<std::uint16_t, MajorVersion<std::uint8_t>, NoMinorVersion>;

  const OpcodeT opcode{1, 2, 3};
  REQUIRE(opcode.id() == 1);
  REQUIRE((opcode.major().has_value() && opcode.major().value() == 2));
  REQUIRE(!opcode.minor().has_value());

  SECTION("byteSerialize/byteDeserialize Native") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(opcode, writer);
    REQUIRE(buffer.size() == sizeof(std::uint16_t) + sizeof(std::uint8_t));

    OpcodeT deserialized{3, 2, 1};
    REQUIRE(deserialized.id() == 3);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE(!deserialized.minor().has_value());

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader);
    REQUIRE(reader.readed() == sizeof(std::uint16_t) + sizeof(std::uint8_t));
    REQUIRE(deserialized.id() == 1);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE(!deserialized.minor().has_value());
  }

  SECTION("byteSerialize/byteDeserialize Endian") {
    std::endian endian = std::endian::native == std::endian::little ? std::endian::big : std::endian::little;
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(opcode, writer, endian);
    REQUIRE(buffer.size() == sizeof(std::uint16_t) + sizeof(std::uint8_t));

    OpcodeT deserialized{3, 2, 1};
    REQUIRE(deserialized.id() == 3);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE(!deserialized.minor().has_value());

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader, endian);
    REQUIRE(reader.readed() == sizeof(std::uint16_t) + sizeof(std::uint8_t));
    REQUIRE(deserialized.id() == 1);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE(!deserialized.minor().has_value());
  }

  SECTION("jsonSerialize/jsonDeserialize") {
    boost::json::object object{};
    jsonSerialize(opcode, object);
    REQUIRE(object.size() == 2);

    OpcodeT deserialized{3, 2, 1};
    REQUIRE(deserialized.id() == 3);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE(!deserialized.minor().has_value());

    jsonDeserialize(deserialized, object);
    REQUIRE(deserialized.id() == 1);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE(!deserialized.minor().has_value());
  }
}

TEST_CASE("Id, Major and Minor") { // NOLINT
  using Y2KaoZ::Network::Protocol::Opcode::BaseOpcode;
  using Y2KaoZ::Network::Protocol::Opcode::MajorVersion;
  using Y2KaoZ::Network::Protocol::Opcode::MinorVersion;
  using OpcodeT = BaseOpcode<std::uint16_t, MajorVersion<std::uint8_t>, MinorVersion<std::uint8_t>>;

  const OpcodeT opcode{1, 2, 3};
  REQUIRE(opcode.id() == 1);
  REQUIRE((opcode.major().has_value() && opcode.major().value() == 2));
  REQUIRE((opcode.minor().has_value() && opcode.minor().value() == 3));

  SECTION("byteSerialize/byteDeserialize Native") {
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(opcode, writer);
    REQUIRE(buffer.size() == sizeof(std::uint16_t) + sizeof(std::uint8_t) + sizeof(std::uint8_t));

    OpcodeT deserialized{3, 2, 1};
    REQUIRE(deserialized.id() == 3);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE((deserialized.minor().has_value() && deserialized.minor().value() == 1));

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader);
    REQUIRE(reader.readed() == sizeof(std::uint16_t) + sizeof(std::uint8_t) + sizeof(std::uint8_t));
    REQUIRE(deserialized.id() == 1);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE((deserialized.minor().has_value() && deserialized.minor().value() == 3));
  }

  SECTION("byteSerialize/byteDeserialize Endian") {
    std::endian endian = std::endian::native == std::endian::little ? std::endian::big : std::endian::little;
    std::vector<std::byte> buffer;
    Y2KaoZ::Network::Buffers::VectBufferWriter writer(&buffer);
    byteSerialize(opcode, writer, endian);
    REQUIRE(buffer.size() == sizeof(std::uint16_t) + sizeof(std::uint8_t) + sizeof(std::uint8_t));

    OpcodeT deserialized{3, 2, 1};
    REQUIRE(deserialized.id() == 3);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE((deserialized.minor().has_value() && deserialized.minor().value() == 1));

    Y2KaoZ::Network::Buffers::SpanBufferReader reader(buffer);
    byteDeserialize(deserialized, reader, endian);
    REQUIRE(reader.readed() == sizeof(std::uint16_t) + sizeof(std::uint8_t) + sizeof(std::uint8_t));
    REQUIRE(deserialized.id() == 1);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE((deserialized.minor().has_value() && deserialized.minor().value() == 3));
  }

  SECTION("jsonSerialize/jsonDeserialize") {
    boost::json::object object{};
    jsonSerialize(opcode, object);
    REQUIRE(object.size() == 3);

    OpcodeT deserialized{3, 2, 1};
    REQUIRE(deserialized.id() == 3);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE((deserialized.minor().has_value() && deserialized.minor().value() == 1));

    jsonDeserialize(deserialized, object);
    REQUIRE(deserialized.id() == 1);
    REQUIRE((deserialized.major().has_value() && deserialized.major().value() == 2));
    REQUIRE((deserialized.minor().has_value() && deserialized.minor().value() == 3));
  }
}