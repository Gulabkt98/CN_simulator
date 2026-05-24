#include "Bridge.h"
#include <algorithm>

Bridge::Bridge(string id) : Device(id) {}

void Bridge::learnMAC(string srcMAC, int incomingPort) {
    if (macTable.find(srcMAC) == macTable.end()) {
        macTable[srcMAC] = incomingPort;
        cout << "[Bridge " << this->id << "] Learned MAC " << srcMAC 
             << " on port " << incomingPort << endl;
    }
}

int Bridge::lookupPort(string destMAC) {
    auto it = macTable.find(destMAC);
    if (it != macTable.end()) {
        return it->second;
    }
    return -1;  // Unknown MAC, flood to all ports
}

vector<Device*> Bridge::getCollisionDomain() const {
    // In a bridge/switch, each port is a separate collision domain
    // For simplicity: return all directly connected devices
    return getConnections();
}

set<string> Bridge::getBroadcastDomain() const {
    set<string> domain;
    // Traverse the network reachable from this bridge
    // For now, simple: all connected devices that are on same LAN
    for (Device* dev : getConnections()) {
        domain.insert(dev->getId());
    }
    return domain;
}

void Bridge::displayMACTable() const {
    cout << "\n[Bridge " << this->id << " MAC Table]\n";
    if (macTable.empty()) {
        cout << "  (empty)\n";
    } else {
        for (const auto& entry : macTable) {
            cout << "  MAC: " << entry.first << " -> Port " << entry.second << endl;
        }
    }
}

void Bridge::receive(Frame frame, Device* sender) {
    cout << "[Bridge " << this->id << "] Received frame from " 
         << frame.sourceMAC << " to " << frame.destinationMAC << endl;
    
    // Learn source MAC
    int incomingPort = -1;
    vector<Device*> conns = getConnections();
    for (size_t i = 0; i < conns.size(); i++) {
        if (conns[i] == sender) {
            incomingPort = i;
            break;
        }
    }
    if (incomingPort != -1) {
        learnMAC(frame.sourceMAC, incomingPort);
    }
    
    // Lookup destination
    int outPort = lookupPort(frame.destinationMAC);
    
    if (outPort != -1 && outPort < (int)conns.size()) {
        // Known MAC, unicast to that port
        cout << "[Bridge " << this->id << "] Unicasting to port " << outPort << endl;
        conns[outPort]->receive(frame, this);
    } else {
        // Unknown MAC, flood to all ports except incoming
        cout << "[Bridge " << this->id << "] Flooding (unknown MAC)" << endl;
        for (size_t i = 0; i < conns.size(); i++) {
            if ((int)i != incomingPort) {
                conns[i]->receive(frame, this);
            }
        }
    }
}
