
#include "EndDevice.h"
#include "../network/Channel.h"
#include "../network/AckBuffer.h"
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

EndDevice::EndDevice(string id, string mac) : Device(id)
{
    macAddress = mac;
    nextSeq = 0;
    base = 0;
    windowSize = 3;
    expectedSeq = 0;
    lastAckSent = -1;
}

string EndDevice::getMAC()
{
    return macAddress;
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
             << " DATA=" << frame.payload << endl;

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
    if (nextSeq >= base + windowSize)
    {
        cout << "[" << id << "] window full, waiting for ACK\n";
        return;
    }

    Frame frame(macAddress, destMAC, data, nextSeq);
    if (transmitFrame(frame))
    {
        sendWindow.emplace(frame.sequenceNumber, frame);
        nextSeq++;
    }
}

void EndDevice::timeout()
{
    if (sendWindow.empty())
    {
        cout << "[" << id << "] no unacknowledged frames to retransmit\n";
        return;
    }

    cout << "[" << id << "] timeout for SEQ=" << base
         << ", Go-Back-N retransmitting all outstanding frames\n";

    for (const auto& entry : sendWindow)
    {
        if (entry.first >= base)
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

        if (frame.sequenceNumber < base)
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

        base = frame.sequenceNumber + 1;

        cout << "[" << id << "] window slides, new base = "
             << base << endl;

        return;
    }

    cout << "[" << id << "] received frame SEQ="
         << frame.sequenceNumber << " from "
         << sender->getId() << endl;

    // parity check
    int ones = 0;

    for (char c : frame.payload)
    {
        if (c == '1')
            ones++;
    }

    int computedParity = ones % 2;

    if (computedParity != frame.parityBit)
    {

        cout << "[ERROR] Frame corrupted during transmission\n";
        return;
    }

    if (frame.sequenceNumber == expectedSeq)
    {
        cout << "[SUCCESS] In-order frame received correctly\n";
        cout << "Payload: " << frame.payload << endl;

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
