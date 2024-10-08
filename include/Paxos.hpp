#ifndef PAXOS_HPP
#define PAXOS_HPP

#include "Common.hpp"

namespace Paxos::Proposer
{
    template <typename T>
    concept ProposerMessageCodec = requires(
        T t,
        ProposeValueRequest propose_request,
        ProposeValueResponse propose_response,
        Common::ByteList byte_list)
    {
        { t.encode_propose_value_request(propose_request) } -> std::same_as<Common::ByteList>;
        { t.encode_propose_value_response(propose_response) } -> std::same_as<Common::ByteList>;

        { t.decode_propose_value_request(byte_list) } -> std::same_as<ProposeValueRequest>;
        { t.decode_propose_value_response(byte_list) } -> std::same_as<ProposeValueResponse>;
    };

    enum class ProposerMessageType
    {
        PROPOSE_VALUE_REQUEST = 1,
        PROPOSE_VALUE_RESPONSE = 2
    };

    class ProposeValueRequest
    {
    private:
        std::uint32_t request_id_;
        std::uint32_t sequence_number_;
        std::string proposed_value_;

    public:
        ProposeValueRequest(std::uint32_t request_id, std::uint32_t sequence_number, std::string proposed_value);
    };

    class ProposeValueResponse
    {
    private:
        std::uint32_t request_id_;
        bool is_success_;

    public:
        ProposeValueResponse(std::uint32_t request_id, bool is_success);
    };


    class ProposerMain
    {
    public:
        ProposerMain() = default;
        auto run() -> void;
    };
};



namespace Paxos
{

    // template <typename T>
    // concept AcceptorMessageCodec = requires()
    // {

    // };

    // template <typename T>
    // concept LearnerMessageCodec = requires()
    // {

    // };
}

#endif
