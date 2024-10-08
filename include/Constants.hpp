#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <cstddef>

namespace Constants
{
    constexpr std::size_t IPV4_MIN_BLOCK_NUMBER{0};
    constexpr std::size_t IPV4_MAX_BLOCK_NUMBER{3};
    constexpr std::size_t IPV4_BLOCK_COUNT{4};
    constexpr std::size_t EXPECTED_DOTS_IPV4{3};
    constexpr std::size_t IPV4_BLOCK_MAX_VALUE{255};
    constexpr std::size_t IPV4_BLOCK_MIN_VALUE{0};
    constexpr std::size_t MIN_IP_SIZE{7};
    constexpr std::size_t MAX_IP_SIZE{15};
    constexpr std::size_t DEFAULT_ARRAY_SIZE{1024};
    constexpr char IPV4_DELIMITER{'.'};
}

#endif
