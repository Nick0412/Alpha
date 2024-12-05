#ifndef PUT_MESSAGE_REQUEST_HPP
#define PUT_MESSAGE_REQUEST_HPP

#include "Common/Types.hpp"

#include <cstdint>
#include <cctype>
#include <string>

namespace Message
{
    class PutMessageRequest
    {
    private:
        std::uint32_t body_length_;
        std::uint32_t operation_id_;
        std::uint32_t key_length_;
        std::string key_;
        std::uint32_t value_length_;
        std::string value_;

    public:
        PutMessageRequest(const std::string& key, const std::string& value);

        static auto serialize(const PutMessageRequest& request) -> Common::Types::ByteList;
        static auto deserialize(const Common::Types::ByteList& bytes) -> PutMessageRequest;

        auto get_key() const -> std::string;
        auto get_value() const -> std::string;

        auto to_string() const -> std::string;
    };
}

#endif
