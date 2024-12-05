#ifndef LIST_UTILS_H
#define LIST_UTILS_H

#include <string>
#include <vector>

namespace Common::ListUtils
{
    template <typename T>
    auto create_byte_list(const std::string& str) -> std::vector<T>
    {
        std::vector<T> ret;

        for (auto c : str)
        {
            ret.push_back(static_cast<T>(c));
        }

        return ret;
    }
}

#endif