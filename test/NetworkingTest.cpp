#include "Testing.hpp"
#include "Networking.hpp"

TEST(Ipv4IsValid)
{
    Networking::Ipv4ValidatorAndGetter temp{"123.45.0.255"};

    IS_EQUAL(temp.validate_and_get().has_value(), true);
}

TEST(Ipv4IsNotValidIncorrectBlocks)
{
    Networking::Ipv4ValidatorAndGetter temp{"2.34.67.230.100"};

    IS_NOT_EQUAL(temp.validate_and_get().has_value(), true);
}

TEST(Ipv4IsNotValidIncorrectValue)
{
    Networking::Ipv4ValidatorAndGetter temp{"2.34.500.244"};

    IS_NOT_EQUAL(temp.validate_and_get().has_value(), true);
}

TEST(Ipv4IsNotValidIncorrectFormat)
{
    Networking::Ipv4ValidatorAndGetter temp{"1.34s.200.244"};

    IS_NOT_EQUAL(temp.validate_and_get().has_value(), true);
}

TEST(VirtualRouterHasCorrectAttachedDevice)
{
    Networking::VirtualRouter router{};
    auto network_device = router.attach_network_device(Networking::IPv4Address{"1.1.1.1"});

    IS_EQUAL(network_device->get_ip_address().ip_string(), "1.1.1.1");
}

RUN_TESTS();
