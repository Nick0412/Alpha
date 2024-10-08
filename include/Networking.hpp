#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include "Common.hpp"

#include <cstdint>
#include <format>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <sys/socket.h>

namespace Networking
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // CONSTANTS & ALIASES
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    using IpBlocks = std::array<std::uint8_t, Constants::IPV4_BLOCK_COUNT>;
    using ByteList = std::vector<std::uint8_t>;
    using ByteArray = std::array<std::uint8_t, Constants::DEFAULT_ARRAY_SIZE>;
    using Port = std::uint16_t;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // CONCEPTS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    concept DataSocketCompatible = requires(T t, std::uint32_t num_bytes, ByteList byte_list)
    {
        t.receive_bytes(num_bytes);
        t.send_bytes(byte_list);
        t.close();
    };

    template <typename T, typename U>
    concept ServerSocketCompatible = DataSocketCompatible<U> && requires(T t)
    {
        { t.accept_incoming_connection() } -> std::same_as<std::optional<U>>;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ENUMS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class IpType
    {
        IPv4 = AF_INET,
        IPv6 = AF_INET6
    };

    enum class SocketType
    {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // CLASS DEFINITIONS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * This class is used to validate an IPv4 string in the dot notation. If the string is valid then `validate_and_get`
     * will return an IpBlocks type.
     */
    class Ipv4ValidatorAndGetter
    {
    private:
        const std::string ip_string_;
        std::vector<std::string> ip_string_blocks_;

        auto is_block_length_correct() const -> bool;
        auto is_block_value_correct() const -> bool;
        auto convert_string_blocks_to_numerical() const -> IpBlocks;

    public:
        Ipv4ValidatorAndGetter(const std::string& ip_string);
        auto validate_and_get() const -> std::optional<IpBlocks>;
    };

    class IPv4Address
    {
    private:
        IpBlocks ip_blocks_;
    
    public:
        IPv4Address() = delete;
        IPv4Address(std::uint8_t block1, std::uint8_t block2, std::uint8_t block3, std::uint8_t block4);
        IPv4Address(const std::string& ipv4_string);
        auto ip_string() const -> std::string;
        auto set_block(std::size_t block_num, std::uint8_t value) -> void;

        friend auto operator==(const IPv4Address& left, const IPv4Address& right) -> bool;
        
        class Hash
        {
        public:
            auto operator()(const IPv4Address& ipv4) const -> std::size_t;
        };
    };

    class VirtualNetworkDevice;

    class VirtualDataSocket
    {
    private:
        ByteList send_buffer_;
        ByteList receive_buffer_;
        IPv4Address local_address_;
        IPv4Address remote_address_;
        Port local_port_;
        Port remote_port_;
        bool is_open_;
    
    public:

        auto receive_bytes(std::size_t num_bytes) -> void;
        auto send_bytes(const ByteList& byte_list) -> void;
        auto close() -> void;
    };

    class VirtualListeningSocket
    {
    private:
        Port port_;
        bool is_listening_;
        std::shared_ptr<VirtualNetworkDevice> attached_network_device_;

    public:
        VirtualListeningSocket(const Port& port, VirtualNetworkDevice& network_device);
        auto accept_incoming_connection() -> std::shared_ptr<VirtualDataSocket>;
    };

    class VirtualNetworkDevice
    {
    private:
        IPv4Address address_;
        std::unordered_map<Port, std::shared_ptr<VirtualListeningSocket>> port_listening_socket_map_;
        std::unordered_map<Port, std::shared_ptr<VirtualDataSocket>> port_data_socket_map_;

    public:
        VirtualNetworkDevice(const IPv4Address& address);
        auto open_listening_socket(const Port& port) -> std::shared_ptr<VirtualListeningSocket>;
        auto get_ip_address() const -> IPv4Address;
    };

    /**
     * Represents a virtual router to attach virtual network devices to.
     */
    class VirtualRouter
    {
    private:
        std::unordered_map<IPv4Address, std::shared_ptr<VirtualNetworkDevice>, IPv4Address::Hash> ip_device_map_;

    public:
        VirtualRouter();
        auto attach_network_device(const IPv4Address& ip_address) -> std::shared_ptr<VirtualNetworkDevice>;
    };
}

#endif
