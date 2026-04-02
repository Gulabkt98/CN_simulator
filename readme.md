Important Design Choices (very important for future steps)

for compiling :
```
g++ main.cpp devices/*.cpp network/*.cpp -std=c++17 -o simulator.exe
```
for running :
```
.\simulator.exe
```



```
project_CN/
│
├── main.cpp
│
├── devices/
│   ├── Device.h
│   ├── Device.cpp
│   ├── EndDevice.h
│   ├── EndDevice.cpp
│   ├── Hub.h
│   └── Hub.cpp
│
└── network/
    ├── Frame.h
    └── Frame.cpp      
```

step-1
isme hmne device.cpp & device.h build kiye & fir main.cpp me testing kr li.Right now your base Device is doing blind broadcasting (sending to all connections).That’s not realistic. In real networks:
we'll improve it in next step-2.



step-2
isme hmne endDevice.h & endDevice.cpp bnaya, isme device can send data to specific device & can detect invalid path


step-3
isme hamne hub.cpp and hub.h banaya.
You now have a working Physical Layer Simulation supporting:
End devices
Hub
Star topology
Broadcast transmission
This satisfies part of the assignment requirement:
Create a star topology with five end devices connected to a hub and enable communication.



step-4(we did it in 4 parts 4.1 4.2 4.3 and 4.4)
Here we start the work of Data link layer, Frame.h bnaya.
isme hmne framing(srcMAC,destMAC,data) introduce kiya, sbhi devices(endDevice,hub) jo data ke sath kam kr rhe the unhe frame se replace kiya.



step-5
here we create switch.h & switch.cpp
implement macTable display, macAddress learning, performing flooding for the first time and then Unicast forwarding then after.



//step 6 will be to implement error detection using parity bits in the frame structure and simulate error scenarios.



//step-7
we modify EndDevice::send() so the device:
Detects collision
Stops transmission
Waits a random time
Retries sending



//step 8 simple Go-Back-N flow control
Implemented:
sender window using base and next sequence number
cumulative ACK handling
receiver-side expected sequence tracking
duplicate ACK generation for out-of-order frames
timeout-based retransmission of all unacknowledged frames

Current simulator keeps both protocols together:
CSMA/CD for access control before each frame transmission
Go-Back-N for flow control and retransmission after transmission


CSMA/CD protocol flow now implemented in the simulator:

1. Carrier sense:
   An end device first checks whether the shared channel is idle.

2. Multiple access:
   Any connected end device may attempt to transmit when the medium is free.

3. Collision detection:
   If a collision is reported by the channel, the sender stops immediately.

4. Jam signal:
   The channel prints a jam-signal message so the collision is visible in the simulation output.

5. Binary exponential backoff:
   The sender waits for a random number of slot times before retrying.

6. Retry limit:
   After 16 failed collision attempts, the frame is dropped.
