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
               string tgtIP, string tgtMAC = "") 
        : messageType(type), senderIP(srcIP), senderMAC(srcMAC),
          targetIP(tgtIP), targetMAC(tgtMAC) {}
    
    string serialize() const {
        string result = "ARP|";
        result += to_string(messageType) + "|";
        result += senderIP + "|" + senderMAC + "|";
        result += targetIP + "|" + targetMAC;
        return result;
    }
    
    static ARPMessage deserialize(const string& data) {
        ARPMessage msg;
        size_t pos = 4;  // skip "ARP|"
        size_t next;
        
        next = data.find("|", pos);
        msg.messageType = (ARPMessageType)stoi(data.substr(pos, next - pos));
        pos = next + 1;
        
        next = data.find("|", pos);
        msg.senderIP = data.substr(pos, next - pos);
        pos = next + 1;
        
        next = data.find("|", pos);
        msg.senderMAC = data.substr(pos, next - pos);
        pos = next + 1;
        
        next = data.find("|", pos);
        msg.targetIP = data.substr(pos, next - pos);
        pos = next + 1;
        
        msg.targetMAC = data.substr(pos);
        return msg;
    }
};

class ARPCache {
private:
    map<string, string> ipToMacCache;  // IP -> MAC mapping
    
public:
    ARPCache() = default;
    
    void addEntry(string ipAddr, string macAddr) {
        ipToMacCache[ipAddr] = macAddr;
        cout << "[ARP Cache] Added: " << ipAddr << " -> " << macAddr << endl;
    }
    
    string lookup(string ipAddr) const {
        auto it = ipToMacCache.find(ipAddr);
        if (it != ipToMacCache.end()) {
            return it->second;
        }
        return "";  // Not found
    }
    
    bool contains(string ipAddr) const {
        return ipToMacCache.find(ipAddr) != ipToMacCache.end();
    }
    
    void displayCache() const {
        cout << "\n[ARP Cache Contents]\n";
        if (ipToMacCache.empty()) {
            cout << "  (empty)\n";
        } else {
            for (const auto& entry : ipToMacCache) {
                cout << "  " << entry.first << " -> " << entry.second << endl;
            }
        }
    }
};

#endif
