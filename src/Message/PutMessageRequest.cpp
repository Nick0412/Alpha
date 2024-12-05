#include "Common/ByteHelper.hpp"
#include "Message/PutMessageRequest.hpp"

#include <cstdint>
#include <sstream>


namespace Message
{
    PutMessageRequest::PutMessageRequest(const std::string& key, const std::string& value)
    : body_length_{static_cast<std::uint32_t>(4 + 4 + 4 + key.size() + 4 + value.size())},
      operation_id_{1},
      key_length_{static_cast<std::uint32_t>(key.size())},
      key_{key},
      value_length_{static_cast<std::uint32_t>(value.size())},
      value_{value}
    {
    }

    auto PutMessageRequest::serialize(const PutMessageRequest& request) -> Common::Types::ByteList
    {
        Common::Types::ByteList bytes;

        Common::write(bytes, request.body_length_);
        Common::write(bytes, request.operation_id_);
        Common::write(bytes, request.key_length_);
        Common::write(bytes, request.key_);
        Common::write(bytes, request.value_length_);
        Common::write(bytes, request.value_);

        return bytes;
    }

    auto PutMessageRequest::deserialize(const Common::Types::ByteList& bytes) -> PutMessageRequest
    {
        std::size_t index = 0;

        std::uint32_t body_length_ = Common::read_uint32_t(bytes, index);
        index += 4;
        std::uint32_t operation_id_ = Common::read_uint32_t(bytes, index);
        index += 4;
        std::uint32_t key_length_ = Common::read_uint32_t(bytes, index);
        index += 4;
        std::string key = Common::read_string(bytes, index, key_length_);
        index += key_length_;
        std::uint32_t value_length_ = Common::read_uint32_t(bytes, index);
        index += 4;
        std::string value = Common::read_string(bytes, index, value_length_);
        index += value_length_;

        return PutMessageRequest(key, value);
    }

    auto PutMessageRequest::get_key() const -> std::string
    {
        return key_;
    }

    auto PutMessageRequest::get_value() const -> std::string
    {
        return value_;
    }

    auto PutMessageRequest::to_string() const -> std::string
    {
        std::stringstream stream;
        stream << "operation_id: " << operation_id_ << "\n"
               << "key: " << key_ << "\n"
               << "value: " << value_ << "\n";
        return stream.str();
    }
}

