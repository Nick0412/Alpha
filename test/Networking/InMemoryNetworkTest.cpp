#include <Testing.hpp>

#include <Networking/InMemoryNetwork.hpp>

TEST(IPv4Address_IpString)
{
    Networking::IPv4Address address{123,111,222,255};
    auto ip_string = address.ip_string();

    IS_EQUAL(ip_string, "123.111.222.255");
}

TEST(InMemoryNetwork_ListeningSocketCreation)
{
    Networking::InMemoryNetwork network{};
    auto listen_sock = network.create_listening_socket(Networking::IPv4Address{123,123,123,123}, 50'000);
    auto does_contain = network.does_listening_socket_map_contain(Networking::IPv4Address{123,123,123,123}, 50'000);

    IS_EQUAL(does_contain, true);
    IS_EQUAL(listen_sock.has_value(), true);
}

TEST(InMemoryNetwork_ListeningSocketCreationDuplicate)
{
    Networking::InMemoryNetwork network{};
    auto listen_sock_1 = network.create_listening_socket(Networking::IPv4Address{123,123,123,123}, 50'000);
    auto listen_sock_2 = network.create_listening_socket(Networking::IPv4Address{123,123,123,123}, 50'000);

    IS_EQUAL(listen_sock_1.has_value(), true);
    IS_EQUAL(listen_sock_2.has_value(), false);
}


RUN_TESTS()
