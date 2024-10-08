#include "Common.hpp"

namespace Common
{
    auto split_string_on_delimiter(const std::string& str, char delim) -> std::vector<std::string>
    {
        std::vector<std::string> result;
        auto start = str.begin();
        auto end = std::find(start, str.end(), delim);

        while (end != str.end()) 
        {
            result.emplace_back(start, end);
            start = end + 1;
            end = std::find(start, str.end(), delim);
        }
        
        result.emplace_back(start, str.end());
        return result;
    }

    auto is_string_numerical(const std::string& str) -> bool
    {
        return std::all_of(str.begin(), str.end(), [](char c) -> bool {
            return std::isdigit(static_cast<unsigned char>(c));
        });
    }
}
