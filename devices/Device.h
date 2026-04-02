
#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <vector>
#include <string>
#include "../network/Frame.h"   // NEW include

using namespace std;

class Device {
protected:
    string id;
    vector<Device*> connections;

public:
    Device(string id);

    string getId() const;

    void connect(Device* device);

    // UPDATED: now receives Frame
    virtual void receive(Frame frame, Device* sender);

    virtual ~Device() {}
};

#endif