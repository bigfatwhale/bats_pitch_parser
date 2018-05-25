//
// Created by Uncle Chu on 12/4/18.
//
#include <boost/python.hpp>
#include "BATSPitchMsgParser.hpp"
#include "BATSMsgFactory.hpp"

using namespace std;

shared_ptr<BATSMessageBase> BATSPitchMsgParser::parse_msg(const string &input)
{
    shared_ptr<BATSMessageBase> pMsg;
	const char* start = input.data();

    pMsg = parse_msg(start, start + input.length());

    if ( pMsg == nullptr )
        throw std::runtime_error("Error parsing message : " + input );

    return pMsg;
}

shared_ptr<BATSMessageBase>
BATSPitchMsgParser::parse_msg( const char*& start, const char* end )
{
	return BATSMsgFactory<const char*>::createMsg(start[msgtype_idx], start, end);
}

void BATSPitchMsgParser::export_to_python()
{
    boost::python::class_<BATSPitchMsgParser>("BATSPitchMsgParser")
            .def("parse_msg", static_cast<shared_ptr<BATSMessageBase> (BATSPitchMsgParser::*)(const string&) >
					(&BATSPitchMsgParser::parse_msg));

}
