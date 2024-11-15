#include "Networking/Sockets.hpp"
#include <iostream>

namespace Networking
{
    ConnectionDetails::ConnectionDetails(const IPv4Address& local_address, Port local_port, const std::optional<IPv4Address>& remote_address, const std::optional<Port>& remote_port)
    : local_address_{local_address}, local_port_{local_port}, remote_address_{remote_address}, remote_port_{remote_port}
    {

    }

    ConnectionDetails::ConnectionDetails(const IPv4Address& local_address, Port local_port)
    : local_address_{local_address}, local_port_{local_port}, remote_address_{std::nullopt}, remote_port_{std::nullopt}
    {

    }

    auto ConnectionDetails::to_string() const -> std::string
    {
        std::string remote_address_string;
        std::string remote_port_string;
        
        if (!remote_address_.has_value() || !remote_port_.has_value())
        {
            remote_address_string = "";
            remote_port_string = "";
        }
        else
        {
            remote_address_string = remote_address_.value().ip_string();
            remote_port_string = std::to_string(remote_port_.value());
        }

        return std::format("{}:{}::{}:{}", local_address_.ip_string(), local_port_, remote_address_string, remote_port_string);
    }

    auto operator==(const ConnectionDetails& left, const ConnectionDetails& right) -> bool
    {
        return left.local_address_ == right.local_address_ &&
               left.local_port_ == right.local_port_ &&
               left.remote_address_ == right.remote_address_ &&
               left.remote_port_ == right.remote_port_;
    }

    auto ConnectionDetails::Hash::operator()(const ConnectionDetails& details) const -> std::size_t
    {
        return std::hash<std::string>{}(details.to_string());
    }

    InMemoryDataSocket::InMemoryDataSocket(const IPv4Address& local_address, Port local_port, const IPv4Address remote_address, Port remote_port)
    : details_{local_address, local_port, std::optional{remote_address}, std::optional{remote_port}}
    {

    }
        
    auto InMemoryDataSocket::get_n_bytes_from_send_buffer(std::size_t num_bytes) const -> ByteList
    {
        if (send_buffer_.size() < num_bytes)
        {
            return send_buffer_;
        }

        return ByteList{send_buffer_.begin(), send_buffer_.begin() + num_bytes};
    }

    auto InMemoryDataSocket::remove_send_buffer_bytes(std::size_t num_bytes) -> void
    {
        if (num_bytes > send_buffer_.size())
        {
            send_buffer_.clear();
        }
        else
        {
            auto iter = send_buffer_.begin();
            send_buffer_.erase(iter, iter + num_bytes);
        }
    }

    auto InMemoryDataSocket::append_receive_buffer_bytes(const ByteList& bytes) -> void
    {
        receive_buffer_.insert(receive_buffer_.end(), bytes.begin(), bytes.end());
    }

    auto InMemoryDataSocket::receive(std::size_t num_bytes) -> ByteList
    {
        ByteList return_bytes;

        if (num_bytes > receive_buffer_.size())
        {
            return_bytes = receive_buffer_;
            receive_buffer_.clear();
        }
        else
        {
            return_bytes = ByteList{receive_buffer_.begin(), receive_buffer_.begin() + num_bytes};
            receive_buffer_.erase(receive_buffer_.begin(), receive_buffer_.begin() + num_bytes);
        }
        
        return return_bytes;
    }

    auto InMemoryDataSocket::send(const ByteList& byte_list) -> void
    {
        send_buffer_.insert(send_buffer_.end(), byte_list.begin(), byte_list.end());
    }

    auto InMemoryDataSocket::receive_buffer_string() const -> std::string
    {
        std::stringstream stream;
        for (auto& val : receive_buffer_)
        {
            stream << val;
        }

        return stream.str();
    }

    auto InMemoryDataSocket::send_buffer_string() const -> std::string
    {
        std::stringstream stream;
        for (auto& val : send_buffer_)
        {
            stream << val;
        }

        return stream.str();
    }

    InMemoryListeningSocket::InMemoryListeningSocket(const IPv4Address& address, Port port)
    : details_{address, port}
    {

    }
}