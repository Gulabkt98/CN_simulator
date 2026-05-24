#include "Router.h"
#include "NetworkPacket.h"
#include <iostream>

using namespace std;

Router::Router(string id, string mac, string ip) : Device(id)
{
    macAddress = mac;
    ipAddress = ip;
}

string Router::getMAC() const
{
    return macAddress;
}

string Router::getIP() const
{
    return ipAddress;
}

void Router::addRoute(const string& destinationIP, Device* nextHop, const string& destinationMac)
{
    routesByIp[destinationIP] = RouteEntry{nextHop, destinationMac};
}

bool Router::forwardFrame(const Frame& frame, Device* nextHop, const string& destinationMac)
{
    if (nextHop == nullptr)
    {
        cout << "[Router " << id << "] missing next hop for forwarding\n";
        return false;
    }

    Frame forwarded(macAddress, destinationMac, frame.payload, frame.sequenceNumber, frame.isACK);

    cout << "[Router " << id << "] forwarding frame to ["
         << nextHop->getId() << "] as next hop for MAC "
         << destinationMac << endl;

    nextHop->receive(forwarded, this);
    return true;
}

void Router::receive(Frame frame, Device* sender)
{
    cout << "[Router " << id << "] received frame from "
         << sender->getId() << endl;

    if (frame.isACK)
    {
        auto returnPath = returnPathsBySourceMac.find(frame.sourceMAC);

        if (returnPath == returnPathsBySourceMac.end())
        {
            cout << "[Router " << id << "] no return path for ACK from MAC "
                 << frame.sourceMAC << endl;
            return;
        }

        forwardFrame(frame, returnPath->second.nextHop, returnPath->second.destinationMac);
        return;
    }

    NetworkPacket packet;

    if (!NetworkPacket::deserialize(frame.payload, packet))
    {
        cout << "[Router " << id << "] could not decode network packet\n";
        return;
    }

    cout << "[Router " << id << "] routing packet from IP "
         << packet.sourceIP << " to IP " << packet.destinationIP << endl;

    if (packet.destinationIP == ipAddress)
    {
        cout << "[Router " << id << "] packet reached router control plane\n";
        return;
    }

    auto route = routesByIp.find(packet.destinationIP);

    if (route == routesByIp.end())
    {
        cout << "[Router " << id << "] no route for destination IP "
             << packet.destinationIP << endl;
        return;
    }

    if (packet.ttl <= 0)
    {
        cout << "[Router " << id << "] dropped packet because TTL expired\n";
        return;
    }

    NetworkPacket forwardedPacket = packet;
    forwardedPacket.ttl--;
    forwardedPacket.hopCount++;

    Frame forwardedFrame(macAddress,
                        route->second.nextHopMac,
                         forwardedPacket.serialize(),
                         frame.sequenceNumber,
                         false);

        returnPathsBySourceMac[route->second.nextHopMac] = ReturnPath{sender, frame.sourceMAC};

    cout << "[Router " << id << "] learned return path for MAC "
            << route->second.nextHopMac << " back to MAC "
         << frame.sourceMAC << endl;

    route->second.nextHop->receive(forwardedFrame, this);
}