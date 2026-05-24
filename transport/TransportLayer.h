#ifndef TRANSPORT_LAYER_H
#define TRANSPORT_LAYER_H

#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "TransportSegment.h"

using namespace std;

typedef void(*ServiceHandler)(const TransportSegment&, string srcIP, uint16_t srcPort);

class TransportLayer {
private:
    uint16_t localPort;
    map<uint16_t, ServiceHandler> portHandlers;  // Port -> Handler function
    
public:
    TransportLayer(uint16_t port = 5000);
    
    // Register a service handler on a specific port
    void registerService(uint16_t port, ServiceHandler handler);
    
    // Receive and demultiplex segment based on destination port
    void receive(const TransportSegment& segment, string srcIP);
    
    // Send a segment
    TransportSegment createSegment(uint16_t destPort, string payload, 
                                  uint16_t seqNum = 0, bool hasAck = false);
};

#endif
