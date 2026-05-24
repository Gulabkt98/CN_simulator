#ifndef TRANSPORT_SEGMENT_H
#define TRANSPORT_SEGMENT_H

#include <string>
#include <cstring>

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
                    uint16_t seqNum, string data, bool ack = false)
        : sourcePort(srcPort), destinationPort(dstPort),
          sequenceNumber(seqNum), acknowledgmentNumber(0),
          hasAck(ack), payload(data) {}
    
    // Serialize to string for transmission
    string serialize() const {
        string result;
        result += to_string(sourcePort) + "|";
        result += to_string(destinationPort) + "|";
        result += to_string(sequenceNumber) + "|";
        result += to_string(acknowledgmentNumber) + "|";
        result += (hasAck ? "1" : "0") + string("|");
        result += payload;
        return result;
    }
    
    // Deserialize from string
    static TransportSegment deserialize(const string& data) {
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
};

#endif
