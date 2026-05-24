#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H

#include <vector>
#include <string>
#include <iostream>
#include "IPv4CIDR.h"

using namespace std;

struct RouteEntry {
    IPv4CIDR destination;        // CIDR network (e.g., 10.0.0.0/24)
    string nextHopIP;            // IP of next hop
    string nextHopMAC;           // MAC of next hop (for direct delivery)
    int metric;                  // Routing metric (hop count for RIP)
    string interface;            // Interface name (optional)
    
    RouteEntry() : metric(1) {}
    
    RouteEntry(IPv4CIDR dest, string nextIP, string nextMAC, 
                             int met = 1, string iface = "");
};

class RoutingTable {
private:
    vector<RouteEntry> routes;
    
public:
    RoutingTable() = default;
    
    void addRoute(IPv4CIDR destination, string nextHopIP, 
                  string nextHopMAC, int metric = 1);
    
    void removeRoute(IPv4CIDR destination);
    
    // Longest-prefix matching: find the route with the most specific prefix
    RouteEntry* lookup(IPv4Address destIP);
    
    void displayTable() const;
    
    vector<RouteEntry> getRoutes() const;
};

#endif
