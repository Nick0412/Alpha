#include "DnsQuery.hpp"
#include <iostream>

auto InMemoryDnsStorage::get_ips(const std::string& dns_string) const -> IpSet
{
    if (dns_map_.contains(dns_string))
    {
        return dns_map_.find(dns_string)->second;
    }

    return IpSet{};
}

auto InMemoryDnsStorage::add_dns_ip(const std::string& dns_string, const IPv4& ipv4) -> void
{
    if (dns_map_.contains(dns_string))
    {
        dns_map_[dns_string].insert(ipv4);
    }
    else
    {
        dns_map_.insert(std::pair{dns_string, IpSet{ipv4}});
    }
}

auto InMemoryDnsStorage::get_dns_list() const -> DnsList
{
    DnsList result;

    for (auto& elem : dns_map_)
    {
        result.push_back(elem.first);
    }

    return result;
}
