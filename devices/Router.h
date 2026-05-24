#ifndef ROUTER_H
#define ROUTER_H

#include "Device.h"
#include <map>

class Router : public Device {

private:
    struct RouteEntry {
        Device* nextHop;
        string nextHopMac;
    };

    struct ReturnPath {
        Device* nextHop;
        string destinationMac;
    };

    string macAddress;
    string ipAddress;
    map<string, RouteEntry> routesByIp;
    map<string, ReturnPath> returnPathsBySourceMac;

    bool forwardFrame(const Frame& frame, Device* nextHop, const string& destinationMac);

public:
    Router(string id, string mac, string ip = "");

    string getMAC() const;
    string getIP() const;

    void addRoute(const string& destinationIP, Device* nextHop, const string& destinationMac);
    void receive(Frame frame, Device* sender) override;
};

#endif