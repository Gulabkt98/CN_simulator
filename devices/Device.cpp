
#include "Device.h"

Device::Device(string id) {
    this->id = id;
}

string Device::getId() const {
    return id;
}

void Device::connect(Device* device) {
    connections.push_back(device);
}

void Device::receive(Frame frame, Device* sender) {

    cout << "[" << id << "] received frame from ["
         << sender->getId() << "]\n";

}