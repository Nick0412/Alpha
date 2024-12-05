#ifndef BYTE_HELPER
#define BYTE_HELPER

#include "Common/Types.hpp"
#include <cstdint>

namespace Common
{
    auto write(Common::Types::ByteList& bytes, std::uint32_t data) -> void;
    auto write(Common::Types::ByteList& bytes, std::string data) -> void;
    auto read_uint32_t(const Common::Types::ByteList& bytes, std::size_t index) -> std::uint32_t;
    auto read_string(const Common::Types::ByteList& bytes, std::size_t index, std::uint32_t string_length) -> std::string;
}

#endif
