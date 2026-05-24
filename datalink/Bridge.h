#ifndef BRIDGE_H
#define BRIDGE_H

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "../physical/Device.h"

using namespace std;

class Bridge : public Device {
private:
    map<string, int> macTable;  // MAC -> port (interface index)
    
public:
    Bridge(string id);
    
    // Bridge functionality
    void learnMAC(string srcMAC, int incomingPort);
    int lookupPort(string destMAC);
    
    // Collision domain: all devices directly connected to this bridge share one domain
    vector<Device*> getCollisionDomain() const;
    
    // Broadcast domain: all devices reachable through flooding
    set<string> getBroadcastDomain() const;
    
    // Display MAC table
    void displayMACTable() const;
    
    virtual void receive(Frame frame, Device* sender) override;
    
    virtual ~Bridge() {}
};

#endif
