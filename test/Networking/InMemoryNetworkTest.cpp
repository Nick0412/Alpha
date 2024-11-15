#include "Testing.hpp"

#include "Networking/InMemoryNetwork.hpp"

TEST(InMemoryNetwork_FullTest)
{
    Networking::IPv4Address address_1{1, 1, 1, 1};
    Networking::IPv4Address address_2{2, 2, 2, 2};
    Networking::Port port_1{80};
    Networking::Port port_2{90};
    Networking::InMemoryNetwork network;

    network.create_listening_socket(address_1, port_1);
    auto client_socket = network.create_data_socket(address_2, port_2, address_1, port_1).value();
    auto server_socket = network.get_data_socket(Networking::ConnectionDetails{address_1, port_1, std::optional(address_2), std::optional(port_2)}).value();

    Networking::ByteList bytes = {'h', 'i'};
    client_socket->send(bytes);
    network.route_bytes(client_socket, 2);
    auto byte_list = server_socket->receive(2);

    IS_EQUAL(bytes, byte_list);
}

TEST(InMemoryNetwork_FullTest)
{
    Networking::IPv4Address address_1{1, 1, 1, 1};
    Networking::IPv4Address address_2{2, 2, 2, 2};
    Networking::Port port_1{80};
    Networking::Port port_2{90};
    Networking::InMemoryNetwork network;

    network.create_listening_socket(address_1, port_1);
    auto client_socket = network.create_data_socket(address_2, port_2, address_1, port_1).value();
    auto server_socket = network.get_data_socket(Networking::ConnectionDetails{address_1, port_1, std::optional(address_2), std::optional(port_2)}).value();

    Networking::ByteList bytes = {'h', 'i'};
    client_socket->send(bytes);
    network.route_bytes(client_socket, 2);
    auto byte_list = server_socket->receive(2);

    IS_EQUAL(bytes, byte_list);
}

RUN_TESTS()
