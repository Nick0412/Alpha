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
#include <unordered_set>
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

    class ConnectionDetails
    {
    public:
        IPv4Address local_address_;
        Port local_port_;
        std::optional<IPv4Address> remote_address_;
        std::optional<Port> remote_port_;

        ConnectionDetails(const IPv4Address& local_address, Port local_port, const std::optional<IPv4Address>& remote_address, const std::optional<Port>& remote_port);
        ConnectionDetails(const IPv4Address& local_address, Port local_port);

        auto to_string() const -> std::string;

        class Hash
        {
        public:
            auto operator()(const ConnectionDetails& details) -> std::size_t;
        };
    };

    class InMemoryDataSocket
    {
    public:
        IPv4Address local_address_;
        Port local_port_;
        std::size_t local_fd_;

        IPv4Address remote_address_;
        Port remote_port_;
        std::size_t remote_fd_;

        ByteList receive_buffer_;
        ByteList send_buffer_;
    
    public:
        InMemoryDataSocket(const IPv4Address& local_address, Port local_port, std::size_t local_fd, const IPv4Address remote_address, Port remote_port, std::size_t remote_fd);
        
        auto get_remote_port() const -> Port;
        auto get_remote_address() const -> IPv4Address;
        auto get_receive_buffer() const -> ByteList;
        auto get_send_buffer() const -> ByteList;
        auto get_n_bytes_from_send_buffer(std::size_t num_bytes) const -> ByteList;
        auto remove_send_buffer_bytes(std::size_t num_bytes) -> void;
        auto append_receive_buffer_bytes(const ByteList& bytes) -> void;
        auto receive(std::size_t num_bytes) -> ByteList;
        auto send(const ByteList& byte_list) -> void;
    };

    class InMemoryListeningSocket
    {
    private:
        IPv4Address address_;
        Port port_;
        std::size_t fd_;
        // TODO: 
        // queue of accepted data sockets

    public:
        InMemoryListeningSocket(const IPv4Address& address, Port port, std::size_t fd);
    };

    using DataSocketUniquePtr = std::unique_ptr<InMemoryDataSocket>;
    using DataSocketPtr = InMemoryDataSocket*;

    using ListeningSocketUniquePtr = std::unique_ptr<InMemoryListeningSocket>;
    using ListeningSocketPtr = InMemoryListeningSocket*;

    class InMemoryHost
    {
    public:
        IPv4Address ip_address_;
        std::unordered_set<FileDescriptor> fds_;

        std::unordered_map<Port, std::unordered_map<FileDescriptor, ListeningSocketUniquePtr>> listening_sockets_;
        std::unordered_map<Port, std::unordered_map<FileDescriptor, DataSocketUniquePtr>> data_sockets_;

        InMemoryHost(const IPv4Address& address);
        auto get_unused_fd() -> FileDescriptor;
    };

    using HostUniquePtr = std::unique_ptr<InMemoryHost>;
    using HostPtr = InMemoryHost*;

    class InMemoryNetwork
    {
    private:
        std::unordered_map<IPv4Address, HostUniquePtr, IPv4Address::Hash> ip_host_mapping_;

    public:
        InMemoryNetwork() = default;

        auto does_listening_socket_map_contain(const IPv4Address& address, Port port, FileDescriptor fd) -> bool;
        auto does_data_socket_map_contain(const IPv4Address& address, Port port, FileDescriptor fd) -> bool;
        auto create_listening_socket(const IPv4Address& address, Port port) -> std::optional<ListeningSocketPtr>;
        auto create_data_socket(const IPv4Address& local_address, Port local_port, const IPv4Address& remote_address, Port remote_port) -> std::optional<DataSocketPtr>;
        auto route_bytes(const IPv4Address& starting_address, Port starting_port, FileDescriptor fd, std::size_t num_bytes) -> void;
        auto add_host_to_network(const IPv4Address& address) -> void;
        auto get_data_socket(const IPv4Address& address, Port port, FileDescriptor fd) -> std::optional<DataSocketPtr>;
    };
}

#endif
