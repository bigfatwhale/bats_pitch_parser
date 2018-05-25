//
// Created by Uncle Chu on 28/4/18.
//

#include "Python.h"
#include "BATSTradingStatusMsg.hpp"
#include "BATSTradeMsg.hpp"
#include "BATSRetailPriceImproveMsg.hpp"
#include "BATSTradeBreakMsg.hpp"
#include "BATSOrderExecutedMsg.hpp"
#include "BATSOrderCancelMsg.hpp"
#include "BATSAuctionUpdateMsg.hpp"
#include "BATSAuctionSummaryMsg.hpp"
#include "BATSAddOrderMsg.hpp"
#include "BATSMessageBase.hpp"
#include "BATSPitchMsgParser.hpp"

BOOST_PYTHON_MODULE(bats_api)
{
    PyEval_InitThreads(); // need this to mess around with GIL

    BATSMessageBase::export_to_python(); // the base class needs to be registered first, otherwise python complains
    BATSTradingStatusMsg::export_to_python();
	BATSTradeMsg::export_to_python();
	BATSRetailPriceImproveMsg::export_to_python();
	BATSTradeBreakMsg::export_to_python();
	BATSOrderExecutedMsg::export_to_python();
    BATSOrderCancelMsg::export_to_python();
    BATSAuctionUpdateMsg::export_to_python();
    BATSAuctionSummaryMsg::export_to_python();
    BATSAddOrderMsg::export_to_python();
    BATSPitchMsgParser::export_to_python();
}