#ifndef ACKBUFFER_H
#define ACKBUFFER_H

#include "Frame.h"
#include <queue>

class AckBuffer {

public:
    static std::queue<Frame> buffer;

};

#endif