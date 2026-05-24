#include "RoutingTable.h"

RouteEntry::RouteEntry(IPv4CIDR dest, string nextIP, string nextMAC,
                       int met, string iface)
    : destination(dest), nextHopIP(nextIP), nextHopMAC(nextMAC),
      metric(met), interface(iface)
{
}

void RoutingTable::addRoute(IPv4CIDR destination, string nextHopIP,
                            string nextHopMAC, int metric)
{
    RouteEntry entry(destination, nextHopIP, nextHopMAC, metric);
    routes.push_back(entry);
    cout << "[Routing Table] Added route: " << destination.toString()
         << " via " << nextHopIP << " (" << nextHopMAC << ")" << endl;
}

void RoutingTable::removeRoute(IPv4CIDR destination)
{
    for (auto it = routes.begin(); it != routes.end(); ++it) {
        if (it->destination == destination) {
            routes.erase(it);
            cout << "[Routing Table] Removed route: "
                 << destination.toString() << endl;
            return;
        }
    }
}

RouteEntry* RoutingTable::lookup(IPv4Address destIP)
{
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

void RoutingTable::displayTable() const
{
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

vector<RouteEntry> RoutingTable::getRoutes() const
{
    return routes;
}