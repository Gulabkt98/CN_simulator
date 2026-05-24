#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

#include "NetworkPacket.h"
#include "../application/ApplicationMessage.h"
#include <map>

class NetworkLayer {

private:
    string localIP;
    map<string, string> routingTable;

public:
    NetworkLayer(string ip = "");

    void setLocalIP(const string& ip);
    string getLocalIP() const;

    void addRoute(const string& destinationIP, const string& nextHopMAC);
    bool hasRoute(const string& destinationIP) const;
    string resolveNextHopMAC(const string& destinationIP) const;

    NetworkPacket buildPacket(const ApplicationMessage& message) const;
    bool isLocalDestination(const NetworkPacket& packet) const;
    NetworkPacket prepareForForwarding(const NetworkPacket& packet) const;
};

#endif
