#ifndef PITCH_SPIRIT_BATSMSGFACTORY_HPP
#define PITCH_SPIRIT_BATSMSGFACTORY_HPP

#include <memory>
#include <string>
#include <type_traits>
#include "BATSMessageBase.hpp"
#include "BATSAddOrderMsg.hpp"
#include "BATSAuctionSummaryMsg.hpp"
#include "BATSAuctionUpdateMsg.hpp"
#include "BATSOrderCancelMsg.hpp"
#include "BATSOrderExecutedMsg.hpp"
#include "BATSRetailPriceImproveMsg.hpp"
#include "BATSTradeBreakMsg.hpp"
#include "BATSTradeMsg.hpp"
#include "BATSTradingStatusMsg.hpp"

template<typename Iter>
class BATSMsgFactory
{
public:

    static std::shared_ptr<BATSMessageBase> createMsg( char msgtype, Iter& start, Iter end );

private:
    using AddOrderMsgDecoder           = BATSAddOrderMsg::add_order_decoder<Iter>;
    using OrderExecutedMsgDecoder      = BATSOrderExecutedMsg::order_executed_decoder<Iter>;
    using OrderCancelMsgDecoder        = BATSOrderCancelMsg::order_cancel_decoder<Iter>;
    using TradeMsgDecoder              = BATSTradeMsg::trade_decoder<Iter>;
    using TradeBreakMsgDecoder         = BATSTradeBreakMsg::trade_break_decoder<Iter>;
    using TradingStatusMsgDecoder      = BATSTradingStatusMsg::trading_status_decoder<Iter>;
    using AuctionUpdateMsgDecoder      = BATSAuctionUpdateMsg::auction_update_decoder<Iter>;
    using AuctionSummaryMsgDecoder     = BATSAuctionSummaryMsg::auction_summary_decoder<Iter>;
    using RetailPriceImproveMsgDecoder = BATSRetailPriceImproveMsg::retail_price_improve_decoder<Iter>;
};

// good template practice below. EnableIfAddOrder/EnableIfTradeMsg/DisableIf are used
// for simulating template function partial specialization (not supported in C++).
// std enable_if allows the use of SFINAE pattern which chooses the special function
// we want to use for BATSAddOrderMsg and BATSTradeMsg. The dummy variable does the magic.
template<typename T>
using EnableIf  = std::enable_if_t<
        std::is_same_v<T, BATSAddOrderMsg> | std::is_same_v<T, BATSTradeMsg>,
        std::shared_ptr<BATSMessageBase> >;

template <typename T>
using DisableIf = std::enable_if_t<
        !( std::is_same_v<T, BATSAddOrderMsg> | std::is_same_v<T, BATSTradeMsg> ),
        std::shared_ptr<BATSMessageBase> >;

template <typename Iter>
struct DecodeHelper {

    template<typename DecodeT, typename MsgT> static
    std::shared_ptr<BATSMessageBase> parse(Iter& start, Iter end, DecodeT &decoder )
    {
        auto data = std::make_shared<MsgT>();

        bool ret = qi::parse(start, end, decoder, *data);
        if (ret)
            return data;
        else
            return nullptr;
    }

    template<typename DecodeT, typename MsgT> static
    DisableIf<MsgT> decode(char msgtype, Iter& start, Iter end)
    {
        static DecodeT decoder( msgtype );
        return DecodeHelper::parse<DecodeT, MsgT>(start, end, decoder);
    }

    template<typename DecodeT, typename MsgT> static
    EnableIf<MsgT> decode(char msgtype, Iter& start, Iter end)
    {
        static DecodeT decoder_short{ MsgT::shortMsgCode };
        static DecodeT decoder_long { MsgT::longMsgCode  };
        return DecodeHelper::parse<DecodeT, MsgT>(
                start, end, ( msgtype == MsgT::shortMsgCode ) ? decoder_short : decoder_long );
    }
};

template <typename Iter>
std::shared_ptr<BATSMessageBase>
BATSMsgFactory<Iter>::createMsg(char msgtype, Iter& start, Iter end)
{
    switch (msgtype)
    {
        case 'A':
        case 'd': {
            return DecodeHelper<Iter>::template decode<AddOrderMsgDecoder, BATSAddOrderMsg>( msgtype, start, end );
            break;
        }
        case 'E': {
            return DecodeHelper<Iter>::template decode<OrderExecutedMsgDecoder, BATSOrderExecutedMsg>( msgtype, start, end);
            break;
        }
        case'X':{
            return DecodeHelper<Iter>::template decode<OrderCancelMsgDecoder, BATSOrderCancelMsg>( msgtype, start, end);
            break;
        }
        case 'P':
        case 'r': {
            return DecodeHelper<Iter>::template decode<TradeMsgDecoder, BATSTradeMsg>( msgtype, start, end);
            break;
        }
        case 'B': {
            return DecodeHelper<Iter>::template decode<TradeBreakMsgDecoder, BATSTradeBreakMsg>( msgtype, start, end);
            break;
        }
        case 'H':{
            return DecodeHelper<Iter>::template decode<TradingStatusMsgDecoder, BATSTradingStatusMsg>( msgtype, start, end);
            break;
        }
        case 'I':{
            return DecodeHelper<Iter>::template decode<AuctionUpdateMsgDecoder, BATSAuctionUpdateMsg>( msgtype, start, end);
            break;
        }
        case 'J':{
            return DecodeHelper<Iter>::template decode<AuctionSummaryMsgDecoder, BATSAuctionSummaryMsg>( msgtype, start, end);
            break;
        }
        case 'R':{
            return DecodeHelper<Iter>::template decode<RetailPriceImproveMsgDecoder, BATSRetailPriceImproveMsg>( msgtype, start, end);
            break;
        }
        default:{
            throw std::runtime_error("Error parsing message = " + std::string(start, end));
        }
    }
}

#endif //PITCH_SPIRIT_BATSMSGFACTORY_HPP
