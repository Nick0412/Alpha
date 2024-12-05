#include "Common/ByteHelper.hpp"

namespace Common
{
    auto write(Common::Types::ByteList& bytes, std::uint32_t data) -> void
    {
        // TODO: put data into network byte order first
        bytes.push_back(0xFF & data);
        bytes.push_back(0xFF & (data >> 8));
        bytes.push_back(0xFF & (data >> 16));
        bytes.push_back(0xFF & (data >> 24));
    }

    auto write(Common::Types::ByteList& bytes, std::string data) -> void
    {
        bytes.insert(bytes.end(), data.begin(), data.end());
    }

    auto read_uint32_t(const Common::Types::ByteList& bytes, std::size_t index) -> std::uint32_t
    {
        std::uint32_t data = 0;

        data |= bytes[index + 3];
        data <<= 8;
        data |= bytes[index + 2];
        data <<= 8;
        data |= bytes[index + 1];
        data <<= 8;
        data |= bytes[index];

        return data;
    }
    auto read_string(const Common::Types::ByteList& bytes, std::size_t index, std::uint32_t string_length) -> std::string
    {
        return std::string(bytes.begin() + index, bytes.begin() + index + string_length);
    }
}
