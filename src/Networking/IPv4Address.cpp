#include "Networking/IPv4Address.hpp"

namespace Networking
{
    IPv4Address::IPv4Address(std::uint8_t block1, std::uint8_t block2, std::uint8_t block3, std::uint8_t block4)
    : ip_blocks_{block1, block2, block3, block4}
    {

    }

    auto IPv4Address::ip_string() const -> std::string
    {
        return std::format("{}.{}.{}.{}", ip_blocks_[0], ip_blocks_[1], ip_blocks_[2], ip_blocks_[3]);
    }

    auto IPv4Address::Hash::operator()(const IPv4Address& ipv4) const -> std::size_t
    {
        auto h1 = std::hash<uint8_t>{}(ipv4.ip_blocks_[0]);
        auto h2 = std::hash<uint8_t>{}(ipv4.ip_blocks_[1]);
        auto h3 = std::hash<uint8_t>{}(ipv4.ip_blocks_[2]);
        auto h4 = std::hash<uint8_t>{}(ipv4.ip_blocks_[3]);

        return h1 ^ h2 ^ h3 ^ h4;
    }

    auto operator==(const IPv4Address& left, const IPv4Address& right) -> bool
    {
        return left.ip_blocks_ == right.ip_blocks_;
    }
}