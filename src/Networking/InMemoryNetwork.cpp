#include "Networking/InMemoryNetwork.hpp"
#include <format>

namespace Networking
{
    auto InMemoryNetwork::create_listening_socket(const IPv4Address& address, Port port) -> std::optional<ListeningSocketPtr>
    {
        ConnectionDetails details{address, port};

        if (listening_map_.contains(details))
        {
            return std::nullopt;
        }

        listening_map_[details] = std::make_unique<InMemoryListeningSocket>(address, port);

        return std::optional(listening_map_[details].get());
    }

    auto InMemoryNetwork::create_data_socket(const IPv4Address& local_address, Port local_port, const IPv4Address& remote_address, Port remote_port) -> std::optional<DataSocketPtr>
    {
        // Check if there is a listening socket locally so we don't have a local data socket there as well.
        ConnectionDetails local_details{local_address, local_port};
        if (listening_map_.contains(local_details))
        {
            return std::nullopt;
        }

        // Check that there is a listening socket on the remote end. If not return nullopt.
        ConnectionDetails remote_details{remote_address, remote_port};
        if (!listening_map_.contains(remote_details))
        {
            return std::nullopt;
        }

        // Make sure no other data socket exists with this same info.
        ConnectionDetails client{local_address, local_port, std::optional{remote_address}, std::optional{remote_port}};
        if (data_map_.contains(client))
        {
            return std::nullopt;
        }

        ConnectionDetails server{remote_address, remote_port, std::optional{local_address}, std::optional{local_port}};
        if (data_map_.contains(server))
        {
            return std::nullopt;
        }

        data_map_[client] = std::make_unique<InMemoryDataSocket>(local_address, local_port, remote_address, remote_port);
        data_map_[server] = std::make_unique<InMemoryDataSocket>(remote_address, remote_port, local_address, local_port);

        return std::optional(data_map_[client].get());
    }

    auto InMemoryNetwork::route_bytes(DataSocketPtr data_socket, std::size_t num_bytes) -> void
    {
        // Put bytes of send buffer from client to the receive buffer on the server
        ConnectionDetails client_details = data_socket->details_;
        ConnectionDetails server_details{client_details.remote_address_.value(), client_details.remote_port_.value(), std::optional(client_details.local_address_), std::optional(client_details.local_port_)};

        auto client = get_data_socket(client_details);
        auto server = get_data_socket(server_details);

        if (!server.has_value() || !client.has_value())
        {
            return;
        }

        auto byte_list = client.value()->get_n_bytes_from_send_buffer(num_bytes);
        client.value()->remove_send_buffer_bytes(num_bytes);
        server.value()->append_receive_buffer_bytes(byte_list);
    }

    auto InMemoryNetwork::get_data_socket(const ConnectionDetails& details) -> std::optional<DataSocketPtr>
    {
        if (!data_map_.contains(details))
        {
            return std::nullopt;
        }

        return std::optional(data_map_[details].get());
    }

    auto InMemoryNetwork::to_string() const -> std::string
    {
        std::stringstream stream;

        stream << "====== Data Map ======\n";
    
        for (auto& [key, value] : data_map_)
        {
            stream << key.to_string() << "\n" 
                   << "Receive{" << value->receive_buffer_string() << "}\n"
                   << "Send{" << value->send_buffer_string() << "}\n"
                   << "\n";
        }

        stream << "====== Listen Map ======\n";

        for (auto& [key, value] : listening_map_)
        {
            stream << key.to_string() << "\n"
                   << "\n";
        }

        return stream.str();
    }
}
