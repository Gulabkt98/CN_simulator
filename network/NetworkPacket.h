#ifndef NETWORKPACKET_H
#define NETWORKPACKET_H

#include <sstream>
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
                  int hops = 0)
    {
        sourceIP = src;
        destinationIP = dest;
        payload = data;
        ttl = timeToLive;
        hopCount = hops;
    }

    string serialize() const
    {
        ostringstream out;
        out << "NETPKT\n"
            << sourceIP << '\n'
            << destinationIP << '\n'
            << ttl << '\n'
            << hopCount << '\n'
            << payload.size() << '\n'
            << payload;

        return out.str();
    }

    static bool deserialize(const string& encoded, NetworkPacket& packet)
    {
        istringstream input(encoded);
        string marker;
        string source;
        string destination;
        string ttlLine;
        string hopLine;
        string payloadSizeLine;

        if (!getline(input, marker) || marker != "NETPKT")
        {
            return false;
        }

        if (!getline(input, source) ||
            !getline(input, destination) ||
            !getline(input, ttlLine) ||
            !getline(input, hopLine) ||
            !getline(input, payloadSizeLine))
        {
            return false;
        }

        try
        {
            int parsedTtl = stoi(ttlLine);
            int parsedHops = stoi(hopLine);
            size_t payloadSize = static_cast<size_t>(stoul(payloadSizeLine));
            string payload;

            payload.resize(payloadSize);

            if (payloadSize > 0)
            {
                input.read(&payload[0], static_cast<std::streamsize>(payloadSize));

                if (static_cast<size_t>(input.gcount()) != payloadSize)
                {
                    return false;
                }
            }

            packet = NetworkPacket(source, destination, payload, parsedTtl, parsedHops);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
};

#endif
