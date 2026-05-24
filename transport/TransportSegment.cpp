#include "TransportSegment.h"

TransportSegment::TransportSegment(uint16_t srcPort, uint16_t dstPort,
                                   uint16_t seqNum, string data, bool ack)
    : sourcePort(srcPort), destinationPort(dstPort),
      sequenceNumber(seqNum), acknowledgmentNumber(0),
      hasAck(ack), payload(data)
{
}

string TransportSegment::serialize() const
{
    string result;
    result += to_string(sourcePort) + "|";
    result += to_string(destinationPort) + "|";
    result += to_string(sequenceNumber) + "|";
    result += to_string(acknowledgmentNumber) + "|";
    result += (hasAck ? "1" : "0") + string("|");
    result += payload;
    return result;
}

TransportSegment TransportSegment::deserialize(const string& data)
{
    TransportSegment seg;
    size_t pos = 0;
    size_t next;

    next = data.find("|", pos);
    seg.sourcePort = stoi(data.substr(pos, next - pos));
    pos = next + 1;

    next = data.find("|", pos);
    seg.destinationPort = stoi(data.substr(pos, next - pos));
    pos = next + 1;

    next = data.find("|", pos);
    seg.sequenceNumber = stoi(data.substr(pos, next - pos));
    pos = next + 1;

    next = data.find("|", pos);
    seg.acknowledgmentNumber = stoi(data.substr(pos, next - pos));
    pos = next + 1;

    next = data.find("|", pos);
    seg.hasAck = (data.substr(pos, next - pos) == "1");
    pos = next + 1;

    seg.payload = data.substr(pos);
    return seg;
}