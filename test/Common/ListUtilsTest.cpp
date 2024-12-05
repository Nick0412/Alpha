#include "Testing.hpp"
#include "Common/ListUtils.hpp"
#include "Common/Types.hpp"

TEST(ListUtils_CreateByteList)
{
    auto byte_list = Common::ListUtils::create_byte_list<std::uint8_t>("dogs");
    Common::Types::ByteList expected = {'d', 'o', 'g', 's'};

    IS_EQUAL(byte_list, expected);
}

RUN_TESTS()