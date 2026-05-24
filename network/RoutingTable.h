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
               int met = 1, string iface = "")
        : destination(dest), nextHopIP(nextIP), nextHopMAC(nextMAC),
          metric(met), interface(iface) {}
};

class RoutingTable {
private:
    vector<RouteEntry> routes;
    
public:
    RoutingTable() = default;
    
    void addRoute(IPv4CIDR destination, string nextHopIP, 
                  string nextHopMAC, int metric = 1) {
        RouteEntry entry(destination, nextHopIP, nextHopMAC, metric);
        routes.push_back(entry);
        cout << "[Routing Table] Added route: " << destination.toString() 
             << " via " << nextHopIP << " (" << nextHopMAC << ")" << endl;
    }
    
    void removeRoute(IPv4CIDR destination) {
        for (auto it = routes.begin(); it != routes.end(); ++it) {
            if (it->destination == destination) {
                routes.erase(it);
                cout << "[Routing Table] Removed route: " 
                     << destination.toString() << endl;
                return;
            }
        }
    }
    
    // Longest-prefix matching: find the route with the most specific prefix
    RouteEntry* lookup(IPv4Address destIP) {
        RouteEntry* bestMatch = nullptr;
        int bestPrefixLength = -1;
        
        for (auto& route : routes) {
            if (route.destination.contains(destIP)) {
                if (route.destination.prefixLength > bestPrefixLength) {
                    bestMatch = &route;
                    bestPrefixLength = route.destination.prefixLength;
                }
            }
        }
        
        return bestMatch;
    }
    
    void displayTable() const {
        cout << "\n========== ROUTING TABLE ==========\n";
        if (routes.empty()) {
            cout << "  (empty)\n";
        } else {
            for (const auto& route : routes) {
                cout << "  Dest: " << route.destination.toString()
                     << " | Next Hop: " << route.nextHopIP
                     << " (" << route.nextHopMAC << ")"
                     << " | Metric: " << route.metric;
                if (!route.interface.empty()) {
                    cout << " | Interface: " << route.interface;
                }
                cout << endl;
            }
        }
        cout << "===================================\n";
    }
    
    vector<RouteEntry> getRoutes() const {
        return routes;
    }
};

#endif
