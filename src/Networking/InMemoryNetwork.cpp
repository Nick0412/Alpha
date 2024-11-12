#include <Networking/InMemoryNetwork.hpp>
#include <format>

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

    InMemoryDataSocket::InMemoryDataSocket(const IPv4Address& local_address, Port local_port, std::size_t local_fd, const IPv4Address remote_address, Port remote_port, std::size_t remote_fd)
    : local_address_{local_address}, remote_address_{remote_address}, local_port_{local_port}, remote_port_{remote_port}, local_fd_{local_fd}, remote_fd_{remote_fd}
    {

    }
        
    auto InMemoryDataSocket::get_remote_port() const -> Port
    {
        return remote_port_;
    }

    auto InMemoryDataSocket::get_remote_address() const -> IPv4Address
    {
        return remote_address_;
    }

    auto InMemoryDataSocket::get_receive_buffer() const -> ByteList
    {
        return receive_buffer_;
    }

    auto InMemoryDataSocket::get_send_buffer() const -> ByteList
    {
        return send_buffer_;
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

    auto InMemoryDataSocket::get_n_bytes_from_send_buffer(std::size_t num_bytes) const -> ByteList
    {
        if (send_buffer_.size() < num_bytes)
        {
            return send_buffer_;
        }

        return ByteList{send_buffer_.begin(), send_buffer_.begin() + num_bytes};
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

    // InMemoryListeningSocket::InMemoryListeningSocket(const IPv4Address& listening_address, Port listening_port, std::size_t file_descriptor_number)
    // : listening_address_{listening_address}, listening_port_{listening_port}, file_descriptor_number_{file_descriptor_number}
    // {

    // }

    // auto InMemoryNetwork::does_listening_socket_map_contain(const IPv4Address& address, Port port) -> bool
    // {   
    //     if (!ip_host_mapping_.contains(address))
    //     {
    //         return false;
    //     }

    //     return ip_host_mapping_[address]->listening_sockets_.contains(port);
    // }

    // auto InMemoryNetwork::does_data_socket_map_contain(const IPv4Address& address, Port port) -> bool
    // {
    //    if (!ip_host_mapping_.contains(address))
    //     {
    //         return false;
    //     }

    //     return ip_host_mapping_[address]->data_sockets_.contains(port);
    // }

    // auto InMemoryNetwork::create_listening_socket(const IPv4Address& address, Port port) -> std::optional<ListeningSocketPtr>
    // {
    //     if (does_data_socket_map_contain(address, port) || does_listening_socket_map_contain(address, port))
    //     {
    //         return std::nullopt;
    //     }
        
    //     std::size_t fd = ip_host_mapping_[address]->get_unused_fd();
    //     ip_host_mapping_[address]->listening_sockets_[port] = std::make_unique<InMemoryListeningSocket>(address, port, fd);
    //     ip_host_mapping_[address]->fds_.insert(fd);

    //     return std::optional<ListeningSocketPtr>(ip_host_mapping_[address]->listening_sockets_[port].get());
    // }

    // // Initially the data socket connects to the listening socket but is then re routed to another data socket.
    // auto InMemoryNetwork::create_data_socket(const IPv4Address& local_address, const IPv4Address& remote_address, Port local_port, Port remote_port) -> std::optional<DataSocketPtr>
    // {
    //     if (!ip_host_mapping_[remote_address]->listening_sockets_.contains(remote_port))
    //     {
    //         return std::nullopt;
    //     }

    //     if (ip_host_mapping_[local_address]->listening_sockets_.contains(local_port) || ip_host_mapping_[local_address]->data_sockets_.contains(local_port))
    //     {
    //         return std::nullopt;
    //     }

    //     std::size_t local_fd = ip_host_mapping_[local_address]->get_unused_fd();
    //     ip_host_mapping_[local_address]->data_sockets_[local_port] = std::make_unique<InMemoryDataSocket>(local_address, remote_address, local_port, remote_port, local_fd);
    //     ip_host_mapping_[local_address]->fds_.insert(local_fd);

    //     std::size_t remote_fd = ip_host_mapping_[remote_address]->get_unused_fd();
    //     ip_host_mapping_[remote_address]->data_sockets_[remote_port] = std::make_unique<InMemoryDataSocket>(remote_address, local_address, remote_port, local_port, remote_fd);
    //     ip_host_mapping_[remote_address]->fds_.insert(remote_fd);

    //     return std::optional<DataSocketPtr>(ip_host_mapping_[local_address]->data_sockets_[local_port].get());

    //     // socket_name      local_ip    local_port      remote_ip   remote_port     fd_number   is_listening
    //     // listening_1      1.1.1.1     10              -------     --------        4           true
    //     // data_client      2.2.2.2     50              1.1.1.1     10              5           false
    //     // data_server      1.1.1.1     10              2.2.2.2     50              5           false     
    //     // data2_client     3.3.3.3     60              1.1.1.1     10              6           false
    //     // data2_server     1.1.1.1     10              3.3.3.3     60              7           false   
    // }

    // // TODO: 
    // auto InMemoryNetwork::route_bytes(const IPv4Address& starting_address, Port starting_port, std::size_t num_bytes) -> void
    // {
    //     if (!ip_host_mapping_.contains(starting_address))
    //     {
    //         return; 
    //     }

    //     auto starting_host = ip_host_mapping_[starting_address].get();

    //     if (!starting_host->data_sockets_.contains(starting_port))
    //     {
    //         return;
    //     }

    //     // We know data socket exists at start address and port

    //     auto starting_socket = starting_host->data_sockets_[starting_port].get();
    //     auto ending_address = starting_socket->remote_address_;
    //     auto ending_port = starting_socket->remote_port_;

    //     if (!ip_host_mapping_.contains(ending_address))
    //     {
    //         return; 
    //     }

    //     auto ending_host = ip_host_mapping_[ending_address].get();

    //     if (!ending_host->data_sockets_.contains(ending_port))
    //     {
    //         return;
    //     }

    //     auto ending_socket = ending_host->data_sockets_[ending_port].get();

    //     // We have a starting socket and ending socket that are right

    //     auto bytes_to_send = starting_socket->get_n_bytes_from_send_buffer(num_bytes);
    //     ending_socket->append_receive_buffer_bytes(bytes_to_send);
    //     starting_socket->remove_send_buffer_bytes(num_bytes);
    // }

    // auto InMemoryHost::get_unused_fd() -> std::size_t
    // {
    //     for (std::size_t i = 0; i < std::numeric_limits<std::size_t>::max(); i++)
    //     {
    //         if (!fds_.contains(i))
    //         {
    //             return i;
    //         }
    //     }
    //     return 0;
    // }

    // InMemoryHost::InMemoryHost(const IPv4Address& address)
    // : ip_address_{address}
    // {

    // }

    // auto InMemoryNetwork::add_host_to_network(const IPv4Address& address) -> void
    // {
    //     if (ip_host_mapping_.contains(address))
    //     {
    //         return;
    //     }

    //     ip_host_mapping_[address] = std::make_unique<InMemoryHost>(address);
    // }

    // auto InMemoryNetwork::get_data_socket(const IPv4Address& address, Port port) -> std::optional<DataSocketPtr>
    // {
    //     if (!ip_host_mapping_.contains(address))
    //     {
    //         return std::nullopt;
    //     }

    //     auto host = ip_host_mapping_[address].get();

    //     if (!host->data_sockets_.contains(port))
    //     {
    //         return std::nullopt;
    //     }

    //     return host->data_sockets_[port].get();
    // }


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

        return std::format("{}:{}:{}:{}", local_address_.ip_string(), local_port_, remote_address_string, remote_port_string);
    }

    auto ConnectionDetails::Hash::operator()(const ConnectionDetails& details) -> std::size_t
    {
        return std::hash<std::string>{}(details.to_string());
    }
}
