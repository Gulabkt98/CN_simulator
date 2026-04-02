
#include "devices/EndDevice.h"
#include "devices/Hub.h"
#include "network/Channel.h"
#include "network/AckBuffer.h"

void deliverACKs(Device* receiver) {

    while(!AckBuffer::buffer.empty()) {

        Frame ack = AckBuffer::buffer.front();
        AckBuffer::buffer.pop();

        receiver->receive(ack, receiver);
    }
}

int main() {

    EndDevice A("A","MAC_A");
    EndDevice B("B","MAC_B");

    Hub H1("H1");

    A.connect(&H1);
    H1.connect(&A);

    B.connect(&H1);
    H1.connect(&B);

    Channel::reset();

    cout << "\n===== CSMA/CD + Go-Back-N Demonstration =====\n\n";

    cout << "--- Test 1: CSMA/CD Normal Transmission ---\n\n";
    A.send("101101","MAC_B");

    cout << "\n--- Deliver ACK ---\n\n";
    deliverACKs(&A);

    cout << "\n--- Test 2: CSMA/CD Carrier Sense (Busy Channel) ---\n\n";
    Channel::busy = true;
    A.send("111000","MAC_B");
    Channel::reset();

    cout << "\n--- Test 3: CSMA/CD Collision Detection + Backoff ---\n\n";
    Channel::scheduleCollision();
    A.send("111000","MAC_B");

    cout << "\n--- Deliver ACK After Collision Recovery ---\n\n";
    deliverACKs(&A);

    cout << "\n--- Test 4: Go-Back-N Send One Full Window ---\n\n";
    A.send("110011","MAC_B");
    A.send("100111","MAC_B");
    A.send("110111","MAC_B");

    cout << "\n--- Test 5: Go-Back-N Window Full, Next Frame Waits ---\n\n";
    A.send("101010","MAC_B");

    cout << "\n--- Test 6: Go-Back-N Timeout Retransmits Outstanding Frames ---\n\n";
    A.timeout();

    cout << "\n--- Delivering Cumulative ACKs ---\n\n";
    deliverACKs(&A);

    cout << "\n--- Test 7: Window Slides, Sender Can Send Next Frame ---\n\n";
    A.send("101010","MAC_B");

    cout << "\n--- Delivering Final ACK ---\n\n";
    deliverACKs(&A);

    return 0;
}
