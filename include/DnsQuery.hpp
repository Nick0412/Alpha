#ifndef DNS_QUERY_HPP
#define DNS_QUERY_HPP

#include "Networking.hpp"

#include <concepts>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
concept HasDnsOperations = requires(T t, std::string dns_string, Networking::IPv4Address ipv4)
{
    { t.get_ips(dns_string) } -> std::same_as<std::vector<Networking::IPv4Address>>;
    { t.add_dns_ip(dns_string, ipv4) } -> std::same_as<void>;
    { t.get_dns_list() } -> std::same_as<std::vector<std::string>>;
};

class InMemoryDnsStorage
{
private:
    using IpSet = std::unordered_set<Networking::IPv4Address, Networking::IPv4Address::Hash>;
    using DnsList = std::vector<std::string>;

    std::unordered_map<std::string, IpSet> dns_map_;

public:
    auto get_ips(const std::string& dns_string) const -> IpSet;
    auto add_dns_ip(const std::string& dns_string, const Networking::IPv4Address& ipv4) -> void;
    auto get_dns_list() const -> DnsList;
};

#endif
