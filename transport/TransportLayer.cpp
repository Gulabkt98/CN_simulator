#include "TransportLayer.h"

TransportLayer::TransportLayer(uint16_t port)
    : localPort(port)
{
}

void TransportLayer::registerService(uint16_t port, ServiceHandler handler)
{
    portHandlers[port] = handler;
    cout << "[Transport Layer] Service registered on port " << port << endl;
}

void TransportLayer::receive(const TransportSegment& segment, string srcIP)
{
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

TransportSegment TransportLayer::createSegment(uint16_t destPort,
                                               string payload,
                                               uint16_t seqNum,
                                               bool hasAck)
{
    return TransportSegment(localPort, destPort, seqNum, payload, hasAck);
}