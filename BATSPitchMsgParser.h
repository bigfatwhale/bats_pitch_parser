//
// Created by Uncle Chu on 12/4/18.
//

#ifndef BATS_PITCH_PARSER_BATSPITCHMSGPARSER_H
#define BATS_PITCH_PARSER_BATSPITCHMSGPARSER_H

#include <memory>
#include <string>
#include "BATSMessageBase.h"

class BATSPitchMsgParser
{
public:
    std::shared_ptr<BATSMessageBase>  parse_msg( const std::string &input );
    static void export_to_python();
private:
    static const uint32_t msgtype_idx{8};
};

#endif //BATS_PITCH_PARSER_BATSPITCHMSGPARSER_H
