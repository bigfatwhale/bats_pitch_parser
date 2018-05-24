//
// Created by Uncle Chu on 10/4/18.
//

#ifndef PITCH_SPIRIT_BATSMSGFACTORY_H
#define PITCH_SPIRIT_BATSMSGFACTORY_H

#include "BATSOrderExecutedMsg.hpp"
#include "BATSAddOrderMsg.hpp"
#include "BATSOrderCancelMsg.hpp"
#include "BATSTradeMsg.hpp"
#include "BATSTradeBreakMsg.hpp"
#include "BATSTradingStatusMsg.hpp"
#include "BATSAuctionUpdateMsg.hpp"
#include "BATSAuctionSummaryMsg.hpp"
#include "BATSRetailPriceImproveMsg.hpp"
#include <string>
#include <memory>

class BATSMessageBase;

template<typename Iter>
class BATSMsgFactory
{
public:

    static std::shared_ptr<BATSMessageBase> createMsg( char msgtype, Iter& start, Iter end );

private:
    // syntatic sugar...
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

class BATSMsgStringFactory : public BATSMsgFactory<const char*>
{
public:
	static std::shared_ptr<BATSMessageBase> createMsg( char msgtype, const std::string& msg );
};


#endif //PITCH_SPIRIT_BATSMSGFACTORY_H
