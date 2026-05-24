#ifndef NETWORKPACKET_H
#define NETWORKPACKET_H

#include <string>

using namespace std;

class NetworkPacket {

public:
    string sourceIP;
    string destinationIP;
    string payload;
    int ttl;
    int hopCount;

    NetworkPacket(string src = "",
                  string dest = "",
                  string data = "",
                  int timeToLive = 16,
                  int hops = 0);

    string serialize() const;

    static bool deserialize(const string& encoded, NetworkPacket& packet);
};

#endif
