#ifndef IN_MEMORY_NETWORK
#define IN_MEMORY_NETWORK

#include "IPv4Address.hpp"
#include "Sockets.hpp"

#include <array>
#include <cstdint>
#include <format>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Networking
{
    class InMemoryNetwork
    {
    private:
        std::unordered_map<ConnectionDetails, DataSocketUniquePtr, ConnectionDetails::Hash> data_map_;
        std::unordered_map<ConnectionDetails, ListeningSocketUniquePtr, ConnectionDetails::Hash> listening_map_;

    public:
        InMemoryNetwork() = default;

        auto create_listening_socket(const IPv4Address& address, Port port) -> std::optional<ListeningSocketPtr>;
        auto create_data_socket(const IPv4Address& local_address, Port local_port, const IPv4Address& remote_address, Port remote_port) -> std::optional<DataSocketPtr>;
        auto route_bytes(DataSocketPtr data_socket, std::size_t num_bytes) -> void;
        auto get_data_socket(const ConnectionDetails& details) -> std::optional<DataSocketPtr>;

        auto to_string() const -> std::string;
    };
}

#endif
