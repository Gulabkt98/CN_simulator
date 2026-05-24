#include "NetworkLayer.h"

NetworkLayer::NetworkLayer(string ip)
{
    localIP = ip;
}

void NetworkLayer::setLocalIP(const string& ip)
{
    localIP = ip;
}

string NetworkLayer::getLocalIP() const
{
    return localIP;
}

void NetworkLayer::addRoute(const string& destinationIP, const string& nextHopMAC)
{
    routingTable[destinationIP] = nextHopMAC;
}

bool NetworkLayer::hasRoute(const string& destinationIP) const
{
    return routingTable.find(destinationIP) != routingTable.end();
}

string NetworkLayer::resolveNextHopMAC(const string& destinationIP) const
{
    auto route = routingTable.find(destinationIP);

    if (route != routingTable.end())
    {
        return route->second;
    }

    return "";
}

NetworkPacket NetworkLayer::buildPacket(const ApplicationMessage& message) const
{
    return NetworkPacket(message.sourceIP, message.destinationIP, message.payload);
}

bool NetworkLayer::isLocalDestination(const NetworkPacket& packet) const
{
    return packet.destinationIP == localIP;
}

NetworkPacket NetworkLayer::prepareForForwarding(const NetworkPacket& packet) const
{
    NetworkPacket forwarded = packet;

    if (forwarded.ttl > 0)
    {
        forwarded.ttl--;
    }

    forwarded.hopCount++;
    return forwarded;
}
