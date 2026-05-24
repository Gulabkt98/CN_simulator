#ifndef ARP_H
#define ARP_H

#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

enum ARPMessageType {
    ARP_REQUEST = 1,
    ARP_REPLY = 2
};

struct ARPMessage {
    ARPMessageType messageType;
    string senderIP;
    string senderMAC;
    string targetIP;
    string targetMAC;  // Empty in requests, filled in replies
    
    ARPMessage() = default;
    
    ARPMessage(ARPMessageType type, string srcIP, string srcMAC, 
               string tgtIP, string tgtMAC = "");

    string serialize() const;

    static ARPMessage deserialize(const string& data);
};

class ARPCache {
private:
    map<string, string> ipToMacCache;  // IP -> MAC mapping
    
public:
    ARPCache() = default;
    
    void addEntry(string ipAddr, string macAddr);

    string lookup(string ipAddr) const;

    bool contains(string ipAddr) const;

    void displayCache() const;
};

#endif
