//
// Created by Chu Ming on 4/11/18.
//

#ifndef PITCH_SPIRIT_BATSTRADINGSTATUSMSG_HPP
#define PITCH_SPIRIT_BATSTRADINGSTATUSMSG_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/python.hpp>
#include <Python.h>
#include <string>
#include <sstream>
#include "BATSMessageBase.hpp"
#include "BATSUtil.hpp"

namespace qi = boost::spirit::qi;
namespace phi = boost::phoenix;

class BATSTradingStatusMsg : public BATSMessageBase
{

public:
    // nested class for decoding the wire msg
    template<typename Iterator>
    struct trading_status_decoder : qi::grammar<Iterator, BATSTradingStatusMsg()>
    {
        trading_status_decoder(char msgtype);
        qi::rule<Iterator, BATSTradingStatusMsg()> m_wire_msg; // member variables
    };

public:
    BATSTradingStatusMsg() : BATSMessageBase() {}
//    BATSTradingStatusMsg(int timestamp, char msgtype, std::string const& symbol) :
//            BATSMessageBase(timestamp, msgtype), m_symbol(symbol) {}

    BATSTradingStatusMsg(int timestamp, char msgtype, std::string const& symbol,
                         char halt_status, uint8_t reg_sho_action,
                         char reserved1, char reserved2):
            BATSMessageBase(timestamp, msgtype),
            m_symbol(symbol),
            m_halt_status(halt_status),
            m_reg_sho_action(reg_sho_action),
            m_reserved1(reserved1),
            m_reserved2(reserved2)
    {}

    std::string repr()
    {
        std::stringstream ss;
        ss << "BATSTradingStatusMsg(timestamp=" << m_timestamp << ", msgtype=" << m_msgtype << ", symbol=" 
            << m_symbol << ", halt_status=" << m_halt_status
            << ", reg_sho_action=" << m_reg_sho_action
            << ", reserved1=" << m_reserved1 << ", reserved2="
            << m_reserved2 << ")";
        return ss.str();
    }
  
    static void export_to_python()
    {
        boost::python::class_<BATSTradingStatusMsg, boost::python::bases<BATSMessageBase>>("BATSTradingStatusMsg")
                .def(boost::python::init<>())
                .def(boost::python::init<int, char, std::string, char, uint8_t, char, char>())
                .def_readwrite("symbol", &BATSTradingStatusMsg::m_symbol)
                .def_readwrite("halt_status", &BATSTradingStatusMsg::m_halt_status)
                .def_readwrite("reg_sho_action", &BATSTradingStatusMsg::m_reg_sho_action)
                .def_readwrite("reserved1", &BATSTradingStatusMsg::m_reserved1)
                .def_readwrite("reserved2", &BATSTradingStatusMsg::m_reserved2)
                .def("__repr__", &BATSTradingStatusMsg::repr);
    }

    std::string m_symbol;
    char m_halt_status;
    uint8_t m_reg_sho_action;
    char m_reserved1;
    char m_reserved2;

    static constexpr auto haltStatus {"HQT"};
};

template<typename Iterator>
BATSTradingStatusMsg::trading_status_decoder<Iterator>::trading_status_decoder(char msgtype) :
        BATSTradingStatusMsg::trading_status_decoder<Iterator>::base_type(m_wire_msg)
{
    // order and execution ids are 12 characters base 36
    qi::uint_parser< uint8_t , 10,   1,  1 > action;
    qi::uint_parser< uint32_t, 10,  10, 10 > m_price;
    qi::uint_parser< uint32_t, 10,   8,  8 > p_ts;

    m_wire_msg = ( p_ts >> qi::char_(msgtype)
                        >> qi::as_string[qi::repeat(8)[qi::char_]]
                        >> qi::char_(BATSTradingStatusMsg::haltStatus)
                        >> action
                        >> qi::char_
                        >> qi::char_ )
                  [qi::_val = phi::construct<BATSTradingStatusMsg>(
                          qi::_1, qi::_2, qi::_3, qi::_4, qi::_5, qi::_6, qi::_7 )];
}

#endif //PITCH_SPIRIT_BATSTRADINGSTATUSMSG_HPP
