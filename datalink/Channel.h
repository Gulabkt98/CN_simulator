#ifndef CHANNEL_H
#define CHANNEL_H

class Device;

class Channel {

public:

    static bool busy;
    static bool collision;
    static Device* activeSender;
    static int scheduledCollisions;

    static bool sense();
    static bool startTransmission(Device* sender);
    static bool detectCollision();
    static void sendJamSignal(Device* sender);
    static void finishTransmission(Device* sender);
    static void abortTransmission(Device* sender);
    static void scheduleCollision(int count = 1);
    static void reset();

};

#endif
