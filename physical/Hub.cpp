
#include "Hub.h"

Hub::Hub(string id) : Device(id) {}

void Hub::receive(Frame frame, Device* sender) {

    cout << "[Hub " << id << "] broadcasting frame\n";

    for (auto device : connections) {

        if (device != sender) {
            device->receive(frame, this);
        }

    }

}
