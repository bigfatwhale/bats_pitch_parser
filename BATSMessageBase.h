//
// Created by Uncle Chu on 9/4/18.
//

#ifndef PITCH_SPIRIT_BATSMESSAGEBASE_H
#define PITCH_SPIRIT_BATSMESSAGEBASE_H

#include <string>

class BATSMessageBase {

public:
    BATSMessageBase();
    BATSMessageBase(int timestamp, char msgtype);
    virtual ~BATSMessageBase();

    std::string repr();
    static void export_to_python();

    int  m_timestamp;
    char m_msgtype;
};

#endif //PITCH_SPIRIT_BATSMESSAGEBASE_H
