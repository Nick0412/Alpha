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

    InMemoryDataSocket::InMemoryDataSocket(const IPv4Address& local_address, const IPv4Address remote_address, Port local_port, Port remote_port)
    : local_address_{local_address}, remote_address_{remote_address}, local_port_{local_port}, remote_port_{remote_port}
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

    InMemoryListeningSocket::InMemoryListeningSocket(const IPv4Address& listening_address, Port listening_port)
    : listening_address_{listening_address}, listening_port_{listening_port}
    {

    }

    auto InMemoryNetwork::does_listening_socket_map_contain(const IPv4Address& address, Port port) -> bool
    {
        if (listening_socket_map_.contains(address))
        {
            if (listening_socket_map_[address].contains(port))
            {
                return true;
            }
        }

        return false;
    }

    auto InMemoryNetwork::does_data_socket_map_contain(const IPv4Address& address, Port port) -> bool
    {
        if (data_socket_map_.contains(address))
        {
            if (data_socket_map_[address].contains(port))
            {
                return true;
            }
        }

        return false;
    }

    auto InMemoryNetwork::create_listening_socket(const IPv4Address& address, Port port) -> std::optional<ListeningSocketPtr>
    {
        if (does_data_socket_map_contain(address, port) || does_listening_socket_map_contain(address, port))
        {
            return std::nullopt;
        }
        
        listening_socket_map_[address][port] = std::make_unique<InMemoryListeningSocket>(address, port);

        return std::optional<ListeningSocketPtr>(listening_socket_map_[address][port].get());
    }

    // Initially the data socket connects to the listening socket but is then re routed to another data socket.
    auto InMemoryNetwork::create_data_socket(const IPv4Address& local_address, const IPv4Address& remote_address, Port local_port, Port remote_port) -> std::optional<DataSocketPtr>
    {
        // There is already a socket (listening or data) bound to the port and address.
        if (does_data_socket_map_contain(local_address, local_port) || does_listening_socket_map_contain(local_address, local_port))
        {
            return std::nullopt;
        }

        if (!does_listening_socket_map_contain(remote_address, remote_port))
        {
            return std::nullopt;
        }

        for (std::uint16_t i = 0; i < std::numeric_limits<std::uint16_t>::max(); i++)
        {
            if (!does_data_socket_map_contain(remote_address, i) && !does_listening_socket_map_contain(remote_address, i))
            {
                data_socket_map_[remote_address][i] = std::make_unique<InMemoryDataSocket>(remote_address, local_address, i, local_port);
                data_socket_map_[local_address][local_port] = std::make_unique<InMemoryDataSocket>(local_address, remote_address, local_port, i);
                return std::optional<DataSocketPtr>(data_socket_map_[local_address][local_port].get());
            }
        }

        return std::nullopt;
    }

    auto InMemoryNetwork::route_bytes(const IPv4Address& starting_address, Port starting_port, std::size_t num_bytes) -> void
    {
        if (!does_data_socket_map_contain(starting_address, starting_port))
        {
            return;
        }

        auto sending_socket_ptr = data_socket_map_[starting_address][starting_port].get();
        auto destination_address = sending_socket_ptr->get_remote_address();
        auto destination_port = sending_socket_ptr->get_remote_port();

        if (!does_data_socket_map_contain(destination_address, destination_port))
        {
            return;
        }

        auto bytes_to_send = sending_socket_ptr->get_n_bytes_from_send_buffer(num_bytes);
        sending_socket_ptr->remove_send_buffer_bytes(num_bytes);

        auto receiving_socket_ptr = data_socket_map_[destination_address][destination_port].get();
        receiving_socket_ptr->append_receive_buffer_bytes(bytes_to_send);
    }
}
