//
// Created by Chu Ming on 4/11/18.
//

#ifndef PITCH_SPIRIT_BATSTRADEBREAKMSG_HPP
#define PITCH_SPIRIT_BATSTRADEBREAKMSG_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/python.hpp>
#include <string>
#include <iostream>
#include "BATSMessageBase.hpp"
#include "BATSUtil.hpp"

namespace qi = boost::spirit::qi;
namespace phi = boost::phoenix;

class BATSTradeBreakMsg : public BATSMessageBase
{
public:
    // nested class for decoding the wire msg
    template<typename Iterator>
    struct trade_break_decoder : qi::grammar<Iterator, BATSTradeBreakMsg()>
    {
        trade_break_decoder(char msgtype);
        qi::rule<Iterator, BATSTradeBreakMsg()> m_wire_msg; // member variables
    };

public:
    BATSTradeBreakMsg(): BATSMessageBase() {}
    BATSTradeBreakMsg(int timestamp, char msgtype, uint64_t execId ) :
            BATSMessageBase(timestamp, msgtype),
            m_execId(execId)
    {
    }

    std::string repr()
    {
        std::stringstream ss;
        ss << "BATSTradeBreakMsg(timestamp=" << m_timestamp << ", msgtype=" << m_msgtype
           << ", oexecId=" << m_execId << ")";
        return ss.str();
    }
  
    static void export_to_python()
    {
        boost::python::class_<BATSTradeBreakMsg, boost::python::bases<BATSMessageBase>>("BATSTradeBreakMsg")
                .def(boost::python::init<>())
                .def(boost::python::init<int, char, uint64_t>())
                .def_readwrite("execId", &BATSTradeBreakMsg::m_execId)
                .def("__repr__", &BATSTradeBreakMsg::repr);
    }

    uint64_t m_execId;
};

template<typename Iterator>
BATSTradeBreakMsg::trade_break_decoder<Iterator>::trade_break_decoder(char msgtype) :
        BATSTradeBreakMsg::trade_break_decoder<Iterator>::base_type(m_wire_msg)
{
    // order and execution ids are 12 characters base 36
    qi::uint_parser< uint64_t, 36, 12, 12 > p_execId;
    qi::uint_parser< uint32_t, 10,  8,  8 > p_ts;

    m_wire_msg  = ( p_ts >> qi::char_(msgtype) >> p_execId )[
            qi::_val = phi::construct<BATSTradeBreakMsg>(qi::_1, qi::_2, qi::_3)];
}

#endif //PITCH_SPIRIT_BATSTRADEBREAKMSG_HPP
