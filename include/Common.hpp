#ifndef COMMON_HPP
#define COMMON_HPP

#include "Constants.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <cctype>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Common
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // CONSTANTS & ALIASES
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    using IpBlocks = std::array<std::uint8_t, Constants::IPV4_BLOCK_COUNT>;
    using ByteList = std::vector<std::uint8_t>;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // CONCEPTS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    concept Serializable = requires(T t, ByteList bytes)
    {
        { T::s_serialize(t) } -> std::same_as<ByteList>;
        { T::s_deserialize(bytes) } -> std::same_as<T>;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // FUNCTIONS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T, typename Elem> requires std::ranges::range<T>
    auto does_iterable_contain(const T& container, const Elem& elem) -> bool
    {
        using container_type = typename T::value_type;
        static_assert(std::is_same_v<Elem, container_type>, "Types are not the same.");

        return std::any_of(container.begin(), container.end(), [elem](const auto& i) {
            return i == elem;
        });
    }

    template <typename T, typename Elem> requires std::ranges::range<T>
    auto count_occurrence_in_iterable(const T& container, const Elem& elem) -> std::size_t
    {
        using container_type = typename T::value_type;
        static_assert(std::is_same_v<Elem, container_type>, "Types are not the same.");

        return std::count(container.begin(), container.end(), elem);
    }

    auto split_string_on_delimiter(const std::string& str, char delim) -> std::vector<std::string>;
    auto is_string_numerical(const std::string& str) -> bool;
}

#endif
