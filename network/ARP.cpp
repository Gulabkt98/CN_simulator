#include "ARP.h"

ARPMessage::ARPMessage(ARPMessageType type, string srcIP, string srcMAC,
                       string tgtIP, string tgtMAC)
    : messageType(type), senderIP(srcIP), senderMAC(srcMAC),
      targetIP(tgtIP), targetMAC(tgtMAC)
{
}

string ARPMessage::serialize() const
{
    string result = "ARP|";
    result += to_string(messageType) + "|";
    result += senderIP + "|" + senderMAC + "|";
    result += targetIP + "|" + targetMAC;
    return result;
}

ARPMessage ARPMessage::deserialize(const string& data)
{
    ARPMessage msg;
    size_t pos = 4;
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

void ARPCache::addEntry(string ipAddr, string macAddr)
{
    ipToMacCache[ipAddr] = macAddr;
    cout << "[ARP Cache] Added: " << ipAddr << " -> " << macAddr << endl;
}

string ARPCache::lookup(string ipAddr) const
{
    auto it = ipToMacCache.find(ipAddr);
    if (it != ipToMacCache.end()) {
        return it->second;
    }
    return "";
}

bool ARPCache::contains(string ipAddr) const
{
    return ipToMacCache.find(ipAddr) != ipToMacCache.end();
}

void ARPCache::displayCache() const
{
    cout << "\n[ARP Cache Contents]\n";
    if (ipToMacCache.empty()) {
        cout << "  (empty)\n";
    } else {
        for (const auto& entry : ipToMacCache) {
            cout << "  " << entry.first << " -> " << entry.second << endl;
        }
    }
}