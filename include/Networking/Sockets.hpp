#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include "IPv4Address.hpp"

#include <memory>
#include <optional>
#include <sstream>

namespace Networking
{
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
        friend auto operator==(const ConnectionDetails& left, const ConnectionDetails& right) -> bool;

        class Hash
        {
        public:
            auto operator()(const ConnectionDetails& details) const -> std::size_t;
        };
    };

    class InMemoryDataSocket
    {
    public:
        ConnectionDetails details_;
        ByteList receive_buffer_;
        ByteList send_buffer_;
    
    public:
        InMemoryDataSocket(const IPv4Address& local_address, Port local_port, const IPv4Address remote_address, Port remote_port);
        
        auto get_n_bytes_from_send_buffer(std::size_t num_bytes) const -> ByteList;
        auto remove_send_buffer_bytes(std::size_t num_bytes) -> void;
        auto append_receive_buffer_bytes(const ByteList& bytes) -> void;
        auto receive(std::size_t num_bytes) -> ByteList;
        auto send(const ByteList& byte_list) -> void;

        auto receive_buffer_string() const -> std::string;
        auto send_buffer_string() const -> std::string;
    };

    class InMemoryListeningSocket
    {
    private:
        ConnectionDetails details_;

    public:
        InMemoryListeningSocket(const IPv4Address& address, Port port);
    };

    using DataSocketUniquePtr = std::unique_ptr<InMemoryDataSocket>;
    using DataSocketPtr = InMemoryDataSocket*;
    using ListeningSocketUniquePtr = std::unique_ptr<InMemoryListeningSocket>;
    using ListeningSocketPtr = InMemoryListeningSocket*;
};

#endif