#include "Channel.h"
#include "../physical/Device.h"
#include <iostream>

bool Channel::busy = false;
bool Channel::collision = false;
Device* Channel::activeSender = nullptr;
int Channel::scheduledCollisions = 0;

bool Channel::sense() {
    return !busy;
}

bool Channel::startTransmission(Device* sender) {

    if (busy && activeSender != sender) {
        return false;
    }

    busy = true;
    activeSender = sender;

    if (scheduledCollisions > 0) {
        collision = true;
        scheduledCollisions--;
    }
    else {
        collision = false;
    }

    return true;
}

bool Channel::detectCollision() {
    return collision;
}

void Channel::sendJamSignal(Device* sender) {
    std::cout << "[Channel] Jam signal sent by ["
              << sender->getId() << "]\n";
}

void Channel::finishTransmission(Device* sender) {

    if (activeSender == sender || activeSender == nullptr) {
        busy = false;
        collision = false;
        activeSender = nullptr;
    }
}

void Channel::abortTransmission(Device* sender) {
    finishTransmission(sender);
}

void Channel::scheduleCollision(int count) {

    if (count > 0) {
        scheduledCollisions += count;
    }
}

void Channel::reset() {
    busy = false;
    collision = false;
    activeSender = nullptr;
    scheduledCollisions = 0;
}
