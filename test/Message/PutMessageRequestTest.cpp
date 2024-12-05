#include "Testing.hpp"
#include "Common/ListUtils.hpp"
#include "Common/Types.hpp"
#include "Message/PutMessageRequest.hpp"

TEST(PutMessageRequest_HappyPath)
{
    std::string key = "my_key";
    std::string value = "my_value";

    auto request = Message::PutMessageRequest(key, value);
    auto bytes = Message::PutMessageRequest::serialize(request);
    auto computed_request = Message::PutMessageRequest::deserialize(bytes);

    IS_EQUAL(request.get_key(), computed_request.get_key());
    IS_EQUAL(request.get_value(), computed_request.get_value());
}

RUN_TESTS()
