#include "Switch.h"

Switch::Switch(string id) : Device(id) {}

void Switch::receive(Frame frame, Device* sender) {

    cout << "[Switch " << id << "] received frame from "
         << sender->getId() << endl;

    // Step 1: MAC Learning
    macTable[frame.sourceMAC] = sender;

    cout << "[Switch " << id << "] learned MAC "
         << frame.sourceMAC << endl;

    // Step 2: Check destination MAC
    if (macTable.find(frame.destinationMAC) != macTable.end()) {

        // Destination known
        Device* target = macTable[frame.destinationMAC];

        cout << "[Switch " << id << "] forwarding frame to "
             << target->getId() << endl;

        target->receive(frame, this);

    }
    else {

        // Destination unknown → flood
        cout << "[Switch " << id << "] destination unknown → flooding\n";

        for (auto device : connections) {

            if (device != sender) {
                device->receive(frame, this);
            }

        }

    }

}

void Switch::printMACTable() {

    cout << "\n[Switch " << id << "] MAC Address Table\n";
    cout << "---------------------------\n";

    for (auto entry : macTable) {

        cout << entry.first << " -> "
             << entry.second->getId() << endl;

    }

    cout << endl;
}