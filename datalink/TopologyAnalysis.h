#ifndef TOPOLOGY_ANALYSIS_H
#define TOPOLOGY_ANALYSIS_H

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <queue>
#include "../physical/Device.h"

using namespace std;

struct DomainReport {
    map<int, set<string>> collisionDomains;  // Domain ID -> set of device IDs
    map<int, set<string>> broadcastDomains;  // Domain ID -> set of device IDs
};

class TopologyAnalyzer {
public:
    // Analyze topology starting from a device and return domain report
    static DomainReport analyzeTopology(Device* startDevice);
    
    // Print the domain analysis report
    static void printDomainReport(const DomainReport& report);
    
    // Get all devices in collision domain (directly connected devices)
    static set<string> getCollisionDomain(Device* device, const vector<Device*>& allDevices);
    
    // Get all devices in broadcast domain (reachable by flooding)
    static set<string> getBroadcastDomain(Device* device, const vector<Device*>& allDevices);
    
private:
    static void dfs(Device* node, set<Device*>& visited, set<string>& deviceIds);
};

#endif
