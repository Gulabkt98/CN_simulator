#ifndef ACKBUFFER_H
#define ACKBUFFER_H

#include "../network/Frame.h"
#include <queue>

class AckBuffer {

public:
    static std::queue<Frame> buffer;

};

#endif