#ifndef FRAME_H
#define FRAME_H

#include <string>

using namespace std;

inline int computeParityBit(const string& data)
{
    int ones = 0;

    for (unsigned char ch : data)
    {
        unsigned char value = ch;

        while (value != 0)
        {
            ones += value & 1;
            value >>= 1;
        }
    }

    return ones % 2;
}

class Frame {

public:

    string sourceMAC;
    string destinationMAC;
    string payload;
    int parityBit;
    int sequenceNumber; 
    bool isACK; 
    

    Frame(string src, string dest, string data,int seq,bool ack=false)
    {
        sourceMAC = src;
        destinationMAC = dest;
        payload = data;
        sequenceNumber = seq;
        isACK = ack;

        parityBit = computeParityBit(payload);
    }

};

#endif