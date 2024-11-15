#ifndef IPV4_ADDRESS_H
#define IPV4_ADDRESS_H

#include <array>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

namespace Networking
{
    using Port = std::uint16_t;
    using ByteList = std::vector<std::uint8_t>;
    using IpBlocks = std::array<std::uint8_t, 4>;
    using FileDescriptor = std::size_t;

    class IPv4Address
    {
    private:
        IpBlocks ip_blocks_;

    public:
        IPv4Address() = delete;
        IPv4Address(std::uint8_t block1, std::uint8_t block2, std::uint8_t block3, std::uint8_t block4);
        IPv4Address(const std::string& ipv4_string);
        auto ip_string() const -> std::string;
        friend auto operator==(const IPv4Address& left, const IPv4Address& right) -> bool;
        
        class Hash
        {
        public:
            auto operator()(const IPv4Address& ipv4) const -> std::size_t;
        };
    };
}

#endif