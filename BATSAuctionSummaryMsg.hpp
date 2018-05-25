//
// Created by Chu Ming on 4/11/18.
//

#ifndef PITCH_SPIRIT_BATSAUCTIONSUMMARYMSG_HPP
#define PITCH_SPIRIT_BATSAUCTIONSUMMARYMSG_HPP


#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/python.hpp>
#include <sstream>
#include "BATSMessageBase.hpp"
#include "BATSUtil.hpp"

namespace qi = boost::spirit::qi;
namespace phi = boost::phoenix;

class BATSAuctionSummaryMsg : public BATSMessageBase
{

public:
    // nested class for decoding the wire msg
    template<typename Iterator>
    struct auction_summary_decoder : qi::grammar<Iterator, BATSAuctionSummaryMsg()>
    {
        auction_summary_decoder(char msgtype);

        qi::rule<Iterator, BATSAuctionSummaryMsg()> m_wire_msg; // member variables
    };

public:
    BATSAuctionSummaryMsg() : BATSMessageBase() {}
    BATSAuctionSummaryMsg(int timestamp, char msgtype, std::string const& symbol, char auction_type,
                          uint64_t price, uint32_t shares) :
            BATSMessageBase(timestamp, msgtype),
            m_symbol(symbol),
            m_auction_type(auction_type),
            m_price(price),
            m_shares(shares)
    {
    }

    std::string repr()
    {
        std::stringstream ss;
        ss << "BATSAuctionSummaryMsg(timestamp=" << m_timestamp << ", msgtype=" << m_msgtype << ", symbol=" 
            << m_symbol << ", auction_type=" << m_auction_type
            << ", price=" << m_price << ", shares=" << m_shares << ")";
        return ss.str();
    }

    static void export_to_python()
    {
        boost::python::class_<BATSAuctionSummaryMsg, boost::python::bases<BATSMessageBase>>("BATSAuctionSummaryMsg")
                .def(boost::python::init<>())
                .def(boost::python::init<int, char, std::string, char, uint64_t, uint32_t>())
                .def_readwrite("symbol", &BATSAuctionSummaryMsg::m_symbol)
                .def_readwrite("auction_type", &BATSAuctionSummaryMsg::m_auction_type)
                .def_readwrite("price", &BATSAuctionSummaryMsg::m_price)
                .def_readwrite("shares", &BATSAuctionSummaryMsg::m_shares)
                .def("__repr__", &BATSAuctionSummaryMsg::repr);
    }

    std::string m_symbol;
    char m_auction_type;
    uint64_t m_price;
    uint32_t m_shares;

    static constexpr auto auctionTypes{"OCHI"};
};

template<typename Iterator>
BATSAuctionSummaryMsg::auction_summary_decoder<Iterator>::auction_summary_decoder(char msgtype) :
        BATSAuctionSummaryMsg::auction_summary_decoder<Iterator>::base_type(m_wire_msg)
{
    // order and execution ids are 12 characters base 36
    qi::uint_parser< uint32_t, 10, 10, 10 > p_shares;
    qi::uint_parser< uint32_t, 10, 10, 10 > m_price;
    qi::uint_parser< uint32_t, 10,  8,  8 > p_ts;

    m_wire_msg = ( p_ts >> qi::char_(msgtype)
                        >> qi::as_string[qi::repeat(8)[qi::char_]]
                        >> qi::char_(BATSAuctionSummaryMsg::auctionTypes)
                        >> m_price
                        >> p_shares )
            [qi::_val = phi::construct<BATSAuctionSummaryMsg>(qi::_1, qi::_2, qi::_3, qi::_4, qi::_5, qi::_6)];

}

#endif //PITCH_SPIRIT_BATSAUCTIONSUMMARYMSG_HPP
