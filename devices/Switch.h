#ifndef SWITCH_H
#define SWITCH_H

#include "Device.h"
#include <unordered_map>

class Switch : public Device {

private:

    // MAC Address Table
    unordered_map<string, Device*> macTable;

public:

    Switch(string id);

    void receive(Frame frame, Device* sender) override;
    void printMACTable();

};

#endif