//
// Created by Uncle Chu on 12/4/18.
//

#define BOOST_TEST_MODULE pitch_parser_test
#define BOOST_TEST_DYN_LINK

#include <memory>
#include <string>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>

#include "BATSPitchMsgParser.hpp"
#include "BATSTradeBreakMsg.hpp"
#include "BATSTradingStatusMsg.hpp"
#include "BATSRetailPriceImproveMsg.hpp"
#include "BATSOrderCancelMsg.hpp"
#include "BATSOrderExecutedMsg.hpp"
#include "BATSAuctionSummaryMsg.hpp"
#include "BATSAuctionUpdateMsg.hpp"
#include "BATSAddOrderMsg.hpp"
#include "BATSTradeMsg.hpp"

using namespace std;

using boost::iostreams::mapped_file_source;
using boost::iostreams::stream;

std::shared_ptr<BATSMessageBase> parse( string const& input )
{
    auto parser = std::make_unique<BATSPitchMsgParser>();
    return parser->parse_msg(input);
}

BOOST_AUTO_TEST_SUITE( test_parse_suite )

    BOOST_AUTO_TEST_CASE( test_parse_trade_break )
    {
        auto msg = parse( "28800168B1K27GA00000Y" );

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'B');

        auto tradeBreakMsg = dynamic_pointer_cast<BATSTradeBreakMsg>(msg);
        // int('1K27GA00000Y', 36) -> 204969015920664610 # from python.
        BOOST_TEST( tradeBreakMsg->m_execId == 204969015920664610 );
    }

    BOOST_AUTO_TEST_CASE( test_parse_trade_status )
    {
        auto msg = parse( "28800168HAAPLSPOTT0XY");

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'H');
        auto tradeStatusMsg = dynamic_pointer_cast<BATSTradingStatusMsg>(msg);

        BOOST_TEST( tradeStatusMsg->m_symbol == "AAPLSPOT"); // ticker symbol
        BOOST_TEST( tradeStatusMsg->m_halt_status == 'T'); //halted status
        BOOST_TEST( tradeStatusMsg->m_reserved1 == 'X');
        BOOST_TEST( tradeStatusMsg->m_reserved2 == 'Y');
    }

    BOOST_AUTO_TEST_CASE( test_parse_retail_price_improve )
    {
        auto msg = parse( "28800168RAAPLSPOTS" );

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'R');

        auto retailMsg = dynamic_pointer_cast<BATSRetailPriceImproveMsg>(msg);
        BOOST_TEST( retailMsg->m_symbol == "AAPLSPOT"); // ticker symbol
        BOOST_TEST( retailMsg->m_retail_price_improve == 'S'); // S : sell-side RPI

    }

    BOOST_AUTO_TEST_CASE( test_parse_order_cancel )
    {
        auto msg = parse("28800168X1K27GA00000Y000500");

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'X');

        auto cancelMsg = dynamic_pointer_cast<BATSOrderCancelMsg>(msg);

        BOOST_TEST( cancelMsg->m_orderId == 204969015920664610); // ticker symbol
        BOOST_TEST( cancelMsg->m_shares == 500); // S : sell-side RPI
    }

    BOOST_AUTO_TEST_CASE( test_parse_order_execute )
    {
        auto msg = parse("28800168E1K27GA00000Y0001001K27GA00000K");

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'E');

        auto execMsg = dynamic_pointer_cast<BATSOrderExecutedMsg>(msg);
        BOOST_TEST( execMsg->m_orderId == 204969015920664610); // ticker symbol
        BOOST_TEST( execMsg->m_shares == 100); // S : sell-side RPI
        BOOST_TEST( execMsg->m_execId == 204969015920664596); // S : sell-side RPI

    }

    BOOST_AUTO_TEST_CASE( test_parse_auction_summary )
    {
        auto msg = parse("28800168JAAPLSPOTC00010068000000020000");

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'J');

        auto auctionSummaryMsg = dynamic_pointer_cast<BATSAuctionSummaryMsg>(msg);

        BOOST_TEST( auctionSummaryMsg->m_symbol == "AAPLSPOT");
        BOOST_TEST( auctionSummaryMsg->m_auction_type == 'C');
        BOOST_TEST( auctionSummaryMsg->m_price == 1006800);
        BOOST_TEST( auctionSummaryMsg->m_shares == 20000);

    }

    BOOST_AUTO_TEST_CASE( test_parse_auction_update )
    {
        auto msg = parse("28800168IAAPLSPOTC00010068000000020000000001000000015034000001309800");

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'I');

        auto auctionUpdateMsg = dynamic_pointer_cast<BATSAuctionUpdateMsg>(msg);

        BOOST_TEST( auctionUpdateMsg->m_symbol == "AAPLSPOT");
        BOOST_TEST( auctionUpdateMsg->m_auction_type == 'C');
        BOOST_TEST( auctionUpdateMsg->m_reference_price == 1006800);
        BOOST_TEST( auctionUpdateMsg->m_buyshares == 20000);
        BOOST_TEST( auctionUpdateMsg->m_sellshares == 10000);
        BOOST_TEST( auctionUpdateMsg->m_indicative_price == 1503400);
        BOOST_TEST( auctionUpdateMsg->m_auction_only_price == 1309800);
    }

    BOOST_AUTO_TEST_CASE( test_parse_add_order, * boost::unit_test::tolerance(0.0001)  )
    {
        auto msg = parse("28800168A1K27GA00000YS000100AAPL  0001831900Y");

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'A');

        auto addOrderMsg = dynamic_pointer_cast<BATSAddOrderMsg>(msg);

        BOOST_TEST( addOrderMsg->m_orderId == 204969015920664610);
        BOOST_TEST( addOrderMsg->m_side == 'S');
        BOOST_TEST( addOrderMsg->m_shares == 100);
        BOOST_TEST( addOrderMsg->m_symbol == "AAPL  ");
        BOOST_TEST( addOrderMsg->m_price == 1831900);
        BOOST_TEST( addOrderMsg->m_display == 'Y');

        msg = parse("28800169d1K27GA00000YS000100AAPL  0001831900YBAML");

        BOOST_TEST( msg->m_timestamp == 28800169 );
        BOOST_TEST( msg->m_msgtype == 'd');

        addOrderMsg = dynamic_pointer_cast<BATSAddOrderMsg>(msg);

        BOOST_TEST( addOrderMsg->m_orderId == 204969015920664610);
        BOOST_TEST( addOrderMsg->m_side == 'S');
        BOOST_TEST( addOrderMsg->m_shares == 100);
        BOOST_TEST( addOrderMsg->m_symbol == "AAPL  ");
        BOOST_TEST( addOrderMsg->m_price == 1831900);
        BOOST_TEST( addOrderMsg->m_display == 'Y');
        BOOST_TEST( addOrderMsg->m_partId == "BAML");
    }

    BOOST_AUTO_TEST_CASE( test_parse_trade )
    {
        auto msg = parse("28800168P1K27GA00000YB000300AAPL  00018319001K27GA00000Z");

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'P');

        auto tradeMsg = dynamic_pointer_cast<BATSTradeMsg>(msg);
        BOOST_TEST( tradeMsg->m_orderId == 204969015920664610);
        BOOST_TEST( tradeMsg->m_side == 'B');
        BOOST_TEST( tradeMsg->m_shares == 300);
        BOOST_TEST( tradeMsg->m_symbol == "AAPL  ");
        BOOST_TEST( tradeMsg->m_price == 1831900);
        BOOST_TEST( tradeMsg->m_execId == 204969015920664611); // python : int("1K27GA00000Z", 36) -> 204969015920664611

        msg = parse("28800168r1K27GA00000YB000300AAPLSPOT00018319001K27GA00000Z");

        BOOST_TEST( msg->m_timestamp == 28800168 );
        BOOST_TEST( msg->m_msgtype == 'r');

        tradeMsg = dynamic_pointer_cast<BATSTradeMsg>(msg);
        BOOST_TEST( tradeMsg->m_orderId == 204969015920664610);
        BOOST_TEST( tradeMsg->m_side == 'B');
        BOOST_TEST( tradeMsg->m_shares == 300);
        BOOST_TEST( tradeMsg->m_symbol == "AAPLSPOT");
        BOOST_TEST( tradeMsg->m_price == 1831900);
        BOOST_TEST( tradeMsg->m_execId == 204969015920664611 ); // python : int("1K27GA00000Z", 36) -> 204969015920664611

    }

    BOOST_AUTO_TEST_CASE( test_load_datafile )
    {
        // this is like a smoke test. mass load a large number of messages from file.

        auto parser = std::make_unique<BATSPitchMsgParser>();
        string line;

        mapped_file_source myfile("pitch_example_data");
        BOOST_TEST(myfile.is_open());

		stream<mapped_file_source> ifs(myfile, std::ios::binary);
        while (getline(ifs, line))
        {
            parser->parse_msg(line);
        }
		ifs.close();
        myfile.close();
    }

    BOOST_AUTO_TEST_CASE( test_load_data_100k )
    {
        // this is like a smoke test. mass load a large number of messages from file.

        auto parser = std::make_unique<BATSPitchMsgParser>();
        string line;

        mapped_file_source myfile("pitch_data_100k");
        BOOST_TEST(myfile.is_open());

		stream<mapped_file_source> ifs(myfile, std::ios::binary);
        while (getline(ifs, line))
        {
            parser->parse_msg(line);
        }
		ifs.close();
        myfile.close();
    }

    BOOST_AUTO_TEST_CASE( test_load_data_stream_100k )
    {
		typedef mapped_file_source::iterator mIter;

        mapped_file_source myfile("pitch_data_stream_100k");
        BOOST_TEST(myfile.is_open());

        auto parser = std::make_unique<BATSPitchMsgParser>();

		mIter next = myfile.begin();
		mIter end = myfile.end();

		while (next != end)
        {
            parser->parse_msg(next, end);
        }
        myfile.close();
    }

    BOOST_AUTO_TEST_CASE( test_fail_parse )
    {
        auto bad_msg = "2880016adfadfihiwey8JAAPLSPOTC00010068000000020000";
        BOOST_CHECK_THROW( parse( bad_msg ), std::runtime_error);
    }

BOOST_AUTO_TEST_SUITE_END()
