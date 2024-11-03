#ifndef IN_MEMORY_NETWORK
#define IN_MEMORY_NETWORK

#include <array>
#include <cstdint>
#include <format>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Networking
{
    using Port = std::uint16_t;
    using ByteList = std::vector<std::uint8_t>;
    using IpBlocks = std::array<std::uint8_t, 4>;

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

    class InMemoryDataSocket
    {
    private:
        IPv4Address local_address_;
        IPv4Address remote_address_;
        Port local_port_;
        Port remote_port_;
        ByteList receive_buffer_;
        ByteList send_buffer_;
    
    public:
        InMemoryDataSocket(const IPv4Address& local_address, const IPv4Address remote_address, Port local_port, Port remote_port);
        
        auto get_remote_port() const -> Port;
        auto get_remote_address() const -> IPv4Address;
        auto get_receive_buffer() const -> ByteList;
        auto get_send_buffer() const -> ByteList;
        auto get_n_bytes_from_send_buffer(std::size_t num_bytes) const -> ByteList;
        auto remove_send_buffer_bytes(std::size_t num_bytes) -> void;
        auto append_receive_buffer_bytes(const ByteList& bytes) -> void;
    };

    class InMemoryListeningSocket
    {
    private:
        IPv4Address listening_address_;
        Port listening_port_;

    public:
        InMemoryListeningSocket(const IPv4Address& listening_address, Port listening_port);
    };

    using DataSocketUniquePtr = std::unique_ptr<InMemoryDataSocket>;
    using ListeningSocketUniquePtr = std::unique_ptr<InMemoryListeningSocket>;
    using DataSocketPtr = InMemoryDataSocket*;
    using ListeningSocketPtr = InMemoryListeningSocket*;

    class InMemoryNetwork
    {
    private:
        std::unordered_map<IPv4Address, std::unordered_map<Port, ListeningSocketUniquePtr>, IPv4Address::Hash> listening_socket_map_;
        std::unordered_map<IPv4Address, std::unordered_map<Port, DataSocketUniquePtr>, IPv4Address::Hash> data_socket_map_;

    public:
        InMemoryNetwork() = default;

        auto does_listening_socket_map_contain(const IPv4Address& address, Port port) -> bool;
        auto does_data_socket_map_contain(const IPv4Address& address, Port port) -> bool;
        auto create_listening_socket(const IPv4Address& address, Port port) -> std::optional<ListeningSocketPtr>;
        auto create_data_socket(const IPv4Address& local_address, const IPv4Address& remote_address, Port local_port, Port remote_port) -> std::optional<DataSocketPtr>;
        auto route_bytes(const IPv4Address& starting_address, Port starting_port, std::size_t num_bytes) -> void;
    };
}

#endif
