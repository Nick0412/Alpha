#include "Networking.hpp"

namespace Networking
{
    Ipv4ValidatorAndGetter::Ipv4ValidatorAndGetter(const std::string& ip_string)
    : ip_string_{ip_string},
      ip_string_blocks_{Common::split_string_on_delimiter(ip_string_, Constants::IPV4_DELIMITER)}
    {

    }

    auto Ipv4ValidatorAndGetter::validate_and_get() const -> std::optional<IpBlocks>
    {
        bool is_valid = is_block_length_correct() && is_block_value_correct();
        return is_valid ? std::optional{convert_string_blocks_to_numerical()} : std::nullopt;
    }

    auto Ipv4ValidatorAndGetter::is_block_length_correct() const -> bool
    {
        return ip_string_blocks_.size() == Constants::IPV4_BLOCK_COUNT;
    }

    auto Ipv4ValidatorAndGetter::is_block_value_correct() const -> bool
    {
        for (auto& block : ip_string_blocks_)
        {
            auto block_int_value = std::stoi(block);
            if (!Common::is_string_numerical(block) || block_int_value > 255 || block_int_value < 0)
            {
                return false;
            }
        }
        return true;
    }

    auto Ipv4ValidatorAndGetter::convert_string_blocks_to_numerical() const -> IpBlocks
    {
        IpBlocks result{static_cast<std::uint8_t>(std::stoi(ip_string_blocks_[0])),
                        static_cast<std::uint8_t>(std::stoi(ip_string_blocks_[1])), 
                        static_cast<std::uint8_t>(std::stoi(ip_string_blocks_[2])),
                        static_cast<std::uint8_t>(std::stoi(ip_string_blocks_[3]))};

        return result;
    }

    IPv4Address::IPv4Address(std::uint8_t block1, std::uint8_t block2, std::uint8_t block3, std::uint8_t block4)
    : ip_blocks_{block1, block2, block3, block4}
    {

    }

    // TODO: Implement helpers to convert string to ip address
    IPv4Address::IPv4Address(const std::string& ipv4_string)
    {
        auto optional_ip = Ipv4ValidatorAndGetter{ipv4_string}.validate_and_get();
        if (!optional_ip.has_value())
        {
            throw std::runtime_error{"ERROR IN CONSTRUCTOR."};
        }

        ip_blocks_ = optional_ip.value();
    }

    auto IPv4Address::ip_string() const -> std::string
    {
        return std::format("{}.{}.{}.{}", ip_blocks_[0], ip_blocks_[1], ip_blocks_[2], ip_blocks_[3]);
    }

    auto IPv4Address::set_block(std::size_t block_num, std::uint8_t value) -> void
    {
        if (block_num >= Constants::IPV4_MIN_BLOCK_NUMBER && block_num <= Constants::IPV4_MAX_BLOCK_NUMBER)
        {
            ip_blocks_[block_num] = value;
        }
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

    auto VirtualRouter::attach_network_device(const IPv4Address& address) -> std::shared_ptr<VirtualNetworkDevice>
    {
        if (ip_device_map_.contains(address))
        {
            return nullptr;
        }

        auto network_device = std::make_shared<VirtualNetworkDevice>(address);
        ip_device_map_.insert({address, network_device});
        return network_device;
    }

    auto VirtualNetworkDevice::get_ip_address() const -> IPv4Address
    {
        return address_;
    }

    VirtualRouter::VirtualRouter()
    {

    }

}
