#ifndef TRANSPORT_LAYER_H
#define TRANSPORT_LAYER_H

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
    TransportLayer(uint16_t port = 5000) : localPort(port) {}
    
    // Register a service handler on a specific port
    void registerService(uint16_t port, ServiceHandler handler) {
        portHandlers[port] = handler;
        cout << "[Transport Layer] Service registered on port " << port << endl;
    }
    
    // Receive and demultiplex segment based on destination port
    void receive(const TransportSegment& segment, string srcIP) {
        cout << "[Transport Layer] Received segment on port " 
             << segment.destinationPort << " from " << srcIP 
             << ":" << segment.sourcePort << endl;
        
        auto it = portHandlers.find(segment.destinationPort);
        if (it != portHandlers.end()) {
            cout << "[Transport Layer] Dispatching to service handler" << endl;
            it->second(segment, srcIP, segment.sourcePort);
        } else {
            cout << "[Transport Layer] No handler for port " 
                 << segment.destinationPort << " (port closed)" << endl;
        }
    }
    
    // Send a segment
    TransportSegment createSegment(uint16_t destPort, string payload, 
                                  uint16_t seqNum = 0, bool hasAck = false) {
        return TransportSegment(localPort, destPort, seqNum, payload, hasAck);
    }
};

#endif
