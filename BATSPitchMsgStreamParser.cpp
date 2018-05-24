#include "BATSPitchMsgStreamParser.h"
#include "BATSMsgFactory.h"

using namespace std;

template <typename Iter>
shared_ptr<BATSMessageBase> BATSPitchMsgStreamParser<Iter>::parse_msg( Iter& start, Iter end )
{
	Iter saved = start;

    return BATSMsgFactory<Iter>::createMsg(*(start + msgtype_idx), start, end);
}
