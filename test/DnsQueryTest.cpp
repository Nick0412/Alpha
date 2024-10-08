#include "Testing.hpp"
#include "DnsQuery.hpp"
#include "Common.hpp"

TEST(DnsQueryTestInsertion)
{
    InMemoryDnsStorage dns{};
    Networking::IPv4Address ip{1, 2, 3, 4};
    dns.add_dns_ip("domain", ip);

    auto result = dns.get_ips("domain");
    auto ip_result = *result.begin();

    IS_EQUAL(result.size(), 1);
    IS_EQUAL(ip_result, ip);
}

TEST(DnsQueryTestRetrieveDnsList)
{
    InMemoryDnsStorage dns{};
    Networking::IPv4Address ip{1, 2, 3, 4};
    Networking::IPv4Address ip_2{120, 255, 100, 50};

    dns.add_dns_ip("domain", ip);
    dns.add_dns_ip("url", ip_2);

    auto result = dns.get_dns_list();
    auto contains_1 = Common::does_iterable_contain(result, std::string{"domain"});
    auto contains_2 = Common::does_iterable_contain(result, std::string{"url"});

    IS_EQUAL(result.size(), 2);
    IS_EQUAL(contains_1, true);
    IS_EQUAL(contains_2, true);
}

RUN_TESTS();
