#ifndef BATS_PITCH_PARSER_BATSPITCHMSGPARSER_H
#define BATS_PITCH_PARSER_BATSPITCHMSGPARSER_H

#include <memory>
#include <string>
#include "BATSMessageBase.hpp"

class BATSPitchMsgParser
{
public:
    std::shared_ptr<BATSMessageBase> parse_msg( const std::string &input );
	std::shared_ptr<BATSMessageBase> parse_msg( const char*& start, const char* end );

    static void export_to_python();
private:
    static const uint32_t msgtype_idx{8};
};

#endif //BATS_PITCH_PARSER_BATSPITCHMSGPARSER_H
