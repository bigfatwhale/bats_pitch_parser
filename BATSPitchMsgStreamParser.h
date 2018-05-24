#ifndef BATS_PITCH_PARSER_BATSPITCHMSGSTREAMPARSER_H
#define BATS_PITCH_PARSER_BATSPITCHMSGSTREAMPARSER_H

#include <memory>
#include "BATSMessageBase.h"

template <typename Iter>
class BATSPitchMsgStreamParser
{
public:
    std::shared_ptr<BATSMessageBase>  parse_msg( Iter& start, Iter end );
private:
    static const uint32_t msgtype_idx{8};
};

#endif //BATS_PITCH_PARSER_BATSPITCHMSGSTREAMPARSER_H

