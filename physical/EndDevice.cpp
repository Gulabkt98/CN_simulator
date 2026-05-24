
#include "EndDevice.h"
#include "../datalink/Channel.h"
#include "../datalink/AckBuffer.h"
#include "../network/NetworkPacket.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <windows.h>

using namespace std;

namespace {
    constexpr int MAX_CSMA_CD_ATTEMPTS = 16;
    constexpr int SLOT_TIME_MS = 200;

    int getBackoffSlots(int attemptNumber) {
        static std::mt19937 generator(std::random_device{}());

        int exponent = std::min(attemptNumber, 10);
        int maxSlots = (1 << exponent) - 1;
        std::uniform_int_distribution<int> distribution(0, maxSlots);

        return distribution(generator);
    }
}

namespace {
    string summarizePayload(const string& payload) {
        string preview = payload.substr(0, 48);

        for (char& ch : preview) {
            if (ch == '\n') {
                ch = ' ';
            }
        }

        if (payload.size() > preview.size()) {
            preview += "...";
        }

        return preview;
    }
}

EndDevice::EndDevice(string id, string mac, string ip) : Device(id), applicationLayer(id), networkLayer(ip)
{
    macAddress = mac;
    ipAddress = ip;
    Sn = 0;
    Sf = 0;
    windowSize = 3;
    expectedSeq = 0;
    lastAckSent = -1;
}

string EndDevice::getMAC() const
{
    return macAddress;
}

string EndDevice::getIP() const
{
    return ipAddress;
}

void EndDevice::setIPAddress(const string& ip)
{
    ipAddress = ip;
    networkLayer.setLocalIP(ip);
}

void EndDevice::addRoute(const string& destinationIP, const string& nextHopMAC)
{
    networkLayer.addRoute(destinationIP, nextHopMAC);
}

bool EndDevice::transmitFrame(const Frame& frame, bool retransmission)
{
    int attempts = 0;

    while (attempts < MAX_CSMA_CD_ATTEMPTS)
    {
        if (!Channel::sense())
        {
            cout << "[" << id << "] channel busy, cannot send SEQ="
                 << frame.sequenceNumber << endl;
            return false;
        }

        if (!Channel::startTransmission(this))
        {
            cout << "[" << id << "] lost access to the channel before sending SEQ="
                 << frame.sequenceNumber << endl;
            return false;
        }

        cout << "[" << id << "] "
             << (retransmission ? "retransmitting" : "sending")
             << " frame SEQ=" << frame.sequenceNumber
             << " DEST=" << frame.destinationMAC
               << " DATA=" << summarizePayload(frame.payload) << endl;

        if (Channel::detectCollision())
        {
            attempts++;

            cout << "[" << id << "] collision detected while sending SEQ="
                 << frame.sequenceNumber << endl;

            Channel::sendJamSignal(this);
            Channel::abortTransmission(this);

            if (attempts == MAX_CSMA_CD_ATTEMPTS)
            {
                break;
            }

            int backoffSlots = getBackoffSlots(attempts);
            cout << "[" << id << "] backing off for "
                 << backoffSlots << " slot(s)\n";

            if (backoffSlots > 0)
            {
                Sleep(backoffSlots * SLOT_TIME_MS);
            }

            continue;
        }

        for (auto device : connections)
        {
            device->receive(frame, this);
        }

        Channel::finishTransmission(this);
        return true;
    }

    cout << "[" << id << "] transmission aborted after "
         << MAX_CSMA_CD_ATTEMPTS << " collision attempts\n";

    return false;
}

void EndDevice::queueACK(const string& destMAC, int ackNumber, bool duplicateACK)
{
    Frame ack(macAddress, destMAC, "", ackNumber, true);

    cout << "[" << id << "] "
         << (duplicateACK ? "sending duplicate ACK " : "sending ACK ")
         << ackNumber << endl;

    AckBuffer::buffer.push(ack);
}

void EndDevice::send(string data, string destMAC)
{
    if (Sn >= Sf + windowSize)
    {
        cout << "[" << id << "] window full, waiting for ACK\n";
        return;
    }

    Frame frame(macAddress, destMAC, data, Sn);
    if (transmitFrame(frame))
    {
        sendWindow.emplace(frame.sequenceNumber, frame);
        Sn++;
    }
}

void EndDevice::sendApplicationData(const string& destinationIP, const string& data)
{
    string nextHopMAC = networkLayer.resolveNextHopMAC(destinationIP);

    if (nextHopMAC.empty())
    {
        cout << "[" << id << "] no route to destination IP "
             << destinationIP << endl;
        return;
    }

    ApplicationMessage message = applicationLayer.composeMessage(ipAddress, destinationIP, data);
    NetworkPacket packet = networkLayer.buildPacket(message);

    cout << "[" << id << "] application layer sending to IP "
         << destinationIP << endl;

    send(packet.serialize(), nextHopMAC);
}

void EndDevice::timeout()
{
    if (sendWindow.empty())
    {
        cout << "[" << id << "] no unacknowledged frames to retransmit\n";
        return;
    }

    cout << "[" << id << "] timeout for SEQ=" << Sf
         << ", Go-Back-N retransmitting all outstanding frames\n";

    for (const auto& entry : sendWindow)
    {
        if (entry.first >= Sf)
        {
            if (!transmitFrame(entry.second, true))
            {
                return;
            }
        }
    }
}

void EndDevice::receive(Frame frame, Device *sender)
{

    // ignore frames not meant for this device
    if (frame.destinationMAC != macAddress)
    {
        cout << "[" << id << "] Silently ignoring frame\n";
        return;
    }

    // handle ACK frames
    if (frame.isACK)
    {
        cout << "[" << id << "] received ACK for SEQ="
             << frame.sequenceNumber << endl;

        if (frame.sequenceNumber < Sf)
        {
            cout << "[" << id << "] stale cumulative ACK ignored\n";
            return;
        }

        for (auto it = sendWindow.begin(); it != sendWindow.end(); )
        {
            if (it->first <= frame.sequenceNumber)
            {
                it = sendWindow.erase(it);
            }
            else
            {
                ++it;
            }
        }

        Sf = frame.sequenceNumber + 1;

        cout << "[" << id << "] window slides, new Sf = "
             << Sf << endl;

        return;
    }

    cout << "[" << id << "] received frame SEQ="
         << frame.sequenceNumber << " from "
         << sender->getId() << endl;

    // parity check
    int computedParity = computeParityBit(frame.payload);

    if (computedParity != frame.parityBit)
    {

        cout << "[ERROR] Frame corrupted during transmission\n";
        return;
    }

    if (frame.sequenceNumber == expectedSeq)
    {
        cout << "[SUCCESS] In-order frame received correctly\n";

        NetworkPacket packet;

        if (NetworkPacket::deserialize(frame.payload, packet))
        {
            cout << "[" << id << "] network layer received packet from IP "
                 << packet.sourceIP << " to IP " << packet.destinationIP << endl;

            if (!networkLayer.isLocalDestination(packet))
            {
                cout << "[" << id << "] packet discarded at network layer (wrong destination)\n";
            }
            else
            {
                ApplicationMessage message(packet.sourceIP, packet.destinationIP, packet.payload);
                applicationLayer.deliverMessage(message);
            }
        }
        else
        {
            cout << "Payload: " << frame.payload << endl;
        }

        lastAckSent = frame.sequenceNumber;
        expectedSeq++;

        queueACK(frame.sourceMAC, lastAckSent);
        return;
    }

    cout << "[" << id << "] out-of-order frame discarded. Expected SEQ="
         << expectedSeq << " but got SEQ=" << frame.sequenceNumber << endl;

    if (lastAckSent >= 0)
    {
        queueACK(frame.sourceMAC, lastAckSent, true);
    }
}
