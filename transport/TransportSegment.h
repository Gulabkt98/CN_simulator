#ifndef TRANSPORT_SEGMENT_H
#define TRANSPORT_SEGMENT_H

#include <cstdint>
#include <string>

using namespace std;

// Transport protocol constants
#define PROTO_ECHO    1
#define PROTO_FTP     2
#define PROTO_TCP_LIKE 3

struct TransportSegment {
    uint16_t sourcePort;
    uint16_t destinationPort;
    uint16_t sequenceNumber;
    uint16_t acknowledgmentNumber;
    bool hasAck;
    string payload;
    
    TransportSegment() 
        : sourcePort(0), destinationPort(0), sequenceNumber(0),
          acknowledgmentNumber(0), hasAck(false) {}
    
    TransportSegment(uint16_t srcPort, uint16_t dstPort, 
                    uint16_t seqNum, string data, bool ack = false);

    // Serialize to string for transmission
    string serialize() const;

    // Deserialize from string
    static TransportSegment deserialize(const string& data);
};

#endif
