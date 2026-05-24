#include "TopologyAnalysis.h"
#include <algorithm>

DomainReport TopologyAnalyzer::analyzeTopology(Device* startDevice) {
    DomainReport report;
    vector<Device*> visited;
    queue<Device*> q;
    
    q.push(startDevice);
    visited.push_back(startDevice);
    
    int collisionDomainId = 0;
    int broadcastDomainId = 0;
    
    set<Device*> processedDevices;
    
    while (!q.empty()) {
        Device* current = q.front();
        q.pop();
        
        if (processedDevices.find(current) != processedDevices.end()) {
            continue;
        }
        processedDevices.insert(current);
        
        // Collision domain: all devices connected to the same hub/switch/bridge
        set<string> collisionDomain;
        collisionDomain.insert(current->getId());
        
        // For hub: all connected devices in collision domain
        // For switch/bridge: each port is separate collision domain
        // For simplicity: group by direct connection
        
        // Broadcast domain: all reachable devices
        set<string> broadcastDomain;
        set<Device*> reachable;
        queue<Device*> bfs;
        bfs.push(current);
        reachable.insert(current);
        
        while (!bfs.empty()) {
            Device* node = bfs.front();
            bfs.pop();
            broadcastDomain.insert(node->getId());
            
            // Visit connections
            for (Device* conn : node->getConnections()) {
                if (reachable.find(conn) == reachable.end()) {
                    reachable.insert(conn);
                    bfs.push(conn);
                    
                    if (find(visited.begin(), visited.end(), conn) == visited.end()) {
                        visited.push_back(conn);
                        q.push(conn);
                    }
                }
            }
        }
        
        if (!collisionDomain.empty()) {
            report.collisionDomains[collisionDomainId++] = collisionDomain;
        }
        if (!broadcastDomain.empty()) {
            report.broadcastDomains[broadcastDomainId++] = broadcastDomain;
        }
    }
    
    return report;
}

void TopologyAnalyzer::printDomainReport(const DomainReport& report) {
    cout << "\n========== TOPOLOGY ANALYSIS REPORT ==========\n";
    
    cout << "\n--- COLLISION DOMAINS ---\n";
    int i = 0;
    for (const auto& domain : report.collisionDomains) {
        cout << "Collision Domain " << domain.first << ": ";
        for (const auto& device : domain.second) {
            cout << device << " ";
        }
        cout << endl;
        i++;
    }
    if (i == 0) {
        cout << "(none)\n";
    }
    
    cout << "\n--- BROADCAST DOMAINS ---\n";
    i = 0;
    for (const auto& domain : report.broadcastDomains) {
        cout << "Broadcast Domain " << domain.first << ": ";
        for (const auto& device : domain.second) {
            cout << device << " ";
        }
        cout << endl;
        i++;
    }
    if (i == 0) {
        cout << "(none)\n";
    }
    
    cout << "\n============================================\n\n";
}

set<string> TopologyAnalyzer::getCollisionDomain(Device* device, const vector<Device*>& allDevices) {
    set<string> domain;
    domain.insert(device->getId());
    
    for (Device* other : allDevices) {
        if (device != other) {
            for (Device* conn : device->getConnections()) {
                if (conn == other) {
                    domain.insert(other->getId());
                }
            }
        }
    }
    
    return domain;
}

set<string> TopologyAnalyzer::getBroadcastDomain(Device* device, const vector<Device*>& allDevices) {
    set<string> domain;
    set<Device*> visited;
    queue<Device*> q;
    
    q.push(device);
    visited.insert(device);
    domain.insert(device->getId());
    
    while (!q.empty()) {
        Device* current = q.front();
        q.pop();
        
        for (Device* conn : current->getConnections()) {
            if (visited.find(conn) == visited.end()) {
                visited.insert(conn);
                domain.insert(conn->getId());
                q.push(conn);
            }
        }
    }
    
    return domain;
}

void TopologyAnalyzer::dfs(Device* node, set<Device*>& visited, set<string>& deviceIds) {
    visited.insert(node);
    deviceIds.insert(node->getId());
    
    for (Device* neighbor : node->getConnections()) {
        if (visited.find(neighbor) == visited.end()) {
            dfs(neighbor, visited, deviceIds);
        }
    }
}
