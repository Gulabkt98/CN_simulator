Important Design Choices (very important for future steps)

for compiling :
```
g++ main.cpp devices/*.cpp application/*.cpp network/*.cpp transport/*.cpp -std=c++17 -o simulator.exe
```
for running :
```
for running :
```
.\simulator.exe
```

## **PROJECT COMPLETION STATUS**

All **Submission 1, 2, and 3** requirements have been implemented and tested.

### **SUBMISSION 1: Physical & Data Link Layers** ✅
- **Physical Layer:**
  - Device hierarchy (Device → EndDevice, Hub, Switch, Bridge, Router)
  - Star topology with devices and hubs
  - Frame-based communication

- **Data Link Layer:**
  - Frame structure with parity bits
  - CSMA/CD protocol with collision detection, backoff, 16-retry limit
  - Go-Back-N sliding window flow control with cumulative ACKs
  - Switch with MAC learning and flooding
  - **Bridge class** with MAC table management and forwarding
  - **Topology Analysis:** Collision domain and Broadcast domain reporting

### **SUBMISSION 2: Network Layer** ✅
- **IP Addressing:**
  - IPv4 address parsing (dotted decimal)
  - **CIDR notation support** (/8 to /32 subnet masks)
  - Network membership testing

- **Routing:**
  - **Longest-prefix matching** for most specific route selection
  - Dynamic routing table with CIDR support
  - Route add/remove/lookup operations
  - **Routing Table display** with metrics

- **ARP (Address Resolution Protocol):**
  - ARP cache for IP↔MAC mappings
  - Cache lookup and entry management
  - ARP message serialization

- **Router:**
  - Multi-hop packet forwarding
  - TTL decrement
  - Reverse path learning for ACKs
  - Network packet serialization/deserialization

### **SUBMISSION 3: Transport & Application Layers** ✅
- **Transport Layer:**
  - Transport segments with port numbers (16-bit source and destination ports)
  - Port-based demultiplexing
  - Service handler registration
  - Sliding-window support (reusable from Data Link layer)

- **Application Services:**
  - **Echo Service** (Port 7): Echo-request/echo-reply
  - **File Transfer Service** (Port 21): PUT/GET file operations with content transfer
  - Service multiplexing by port

- **Application Layer:**
  - Application message composition and delivery
  - Multi-layer packet encapsulation (Application → Network → Data Link → Physical)

---

## **PROJECT STRUCTURE**

```
project_CN/
│
├── main.cpp                              (Master demo with 15 tests)
├── readme.md                            (This file)
│
├── devices/                              (Physical + Data Link layer devices)
│   ├── Device.h / Device.cpp            (Base device abstraction)
│   ├── EndDevice.h / EndDevice.cpp      (CSMA/CD + Go-Back-N host)
│   ├── Hub.h / Hub.cpp                  (Broadcast hub)
│   ├── Switch.h / Switch.cpp            (MAC-learning switch)
│   ├── Bridge.h / Bridge.cpp            (Bridge forwarding + domain analysis)
│   └── Router.h / Router.cpp            (IP forwarding device)
│
├── network/                              (Network layer)
│   ├── Frame.h                          (Data link frame)
│   ├── Channel.h / Channel.cpp          (CSMA/CD channel state)
│   ├── AckBuffer.h / AckBuffer.cpp      (ACK queue)
│   ├── NetworkPacket.h / NetworkPacket.cpp (IP packet model)
│   ├── NetworkLayer.h / NetworkLayer.cpp (Packet preparation)
│   ├── TopologyAnalysis.h / TopologyAnalysis.cpp (Collision/broadcast domains)
│   ├── IPv4CIDR.h / IPv4CIDR.cpp        (CIDR parsing and matching)
│   ├── RoutingTable.h / RoutingTable.cpp (Longest-prefix routing table)
│   └── ARP.h / ARP.cpp                  (ARP cache and messages)
│
├── transport/                            (Transport layer)
│   ├── TransportSegment.h / TransportSegment.cpp (Port and sequence fields)
│   └── TransportLayer.h / TransportLayer.cpp     (Port demultiplexing)
│
└── application/                          (Application layer)
  ├── ApplicationMessage.h / ApplicationMessage.cpp (App payload wrapper)
  ├── ApplicationLayer.h / ApplicationLayer.cpp     (Message compose/deliver)
  └── ApplicationServices.h / ApplicationServices.cpp (Echo + FTP handlers)
```

### **Layer-wise file map**

- `devices/` holds the physical and data link device behavior.
- `network/` holds packet, routing, CIDR, and ARP logic.
- `transport/` holds port-based segment handling and demultiplexing.
- `application/` holds application payload objects and service handlers.
- `main.cpp` only wires the layers together and runs the test scenarios.

---

## **KEY FEATURES & CONCEPTS**

### **CSMA/CD (Carrier Sense Multiple Access with Collision Detection)**
- Devices sense carrier before transmission
- Detect collisions and jam signal
- Binary exponential backoff (0 to 2^retries - 1)
- Retry limit: 16 attempts

### **Go-Back-N Flow Control**
- Sender window (base to base+N-1)
- Cumulative ACKs
- Timeout-based retransmission
- Duplicate ACK handling

### **Bridge & Domain Analysis**
- Bridge learns MAC addresses on each port
- Unicast forwarding for known MACs
- Flooding for unknown MACs
- Collision domain: directly connected devices
- Broadcast domain: reachable by flooding

### **CIDR & Longest-Prefix Routing**
- CIDR format: X.X.X.X/Y (Y = prefix bits)
- Subnet mask calculation
- Longest-prefix match for route selection
- More specific routes take priority

### **ARP Protocol**
- IP → MAC address resolution
- Cache-based lookup
- ARP message structure with request/reply types

### **Transport Layer Services**
- Port-based multiplexing (source and destination ports)
- Service handlers registered per port
- Echo service for connectivity testing
- File Transfer service (PUT/GET) for data exchange

### **Multi-Layer Packet Encapsulation**
- Application Message (data)
  → Network Packet (IP header + payload)
    → Frame (MAC header + network packet)
      → Physical transmission

---

## **STEP-BY-STEP IMPLEMENTATION SUMMARY**

| Step | Layer | Feature | Status |
|------|-------|---------|--------|
| 1-3 | Physical | Device, EndDevice, Hub, Star topology | ✅ |
| 4 | Data Link | Frame structure with MAC and data | ✅ |
| 5 | Data Link | Switch with MAC learning & flooding | ✅ |
| 6 | Data Link | Parity bit error detection | ✅ |
| 7 | Data Link | CSMA/CD collision handling | ✅ |
| 8 | Data Link | Go-Back-N sliding window | ✅ |
| 9 | Network | Basic routing & Network layer | ✅ |
| 10 | Network | Multi-hop routing with Router | ✅ |
| **11** | **Network** | **Bridge & Domain Analysis (Submission 1)** | **✅** |
| **12** | **Network** | **CIDR IPv4 & Longest-Prefix (Submission 2)** | **✅** |
| **13** | **Network** | **ARP Protocol (Submission 2)** | **✅** |
| **14** | **Transport** | **Port-Based Demultiplexing (Submission 3)** | **✅** |
| **15** | **Application** | **Echo & File Transfer Services (Submission 3)** | **✅** |

---

## **COMPILATION & EXECUTION**

**Compile:**
```bash
g++ main.cpp devices/*.cpp application/*.cpp network/*.cpp -std=c++17 -o simulator.exe
```

**Run:**
```bash
.\simulator.exe
```

**Expected Output:**
Tests 1-7: CSMA/CD and Go-Back-N legacy protocol tests
Tests 8-9: Multi-hop routing with Network and Application layers
Test 10: Bridge MAC learning and topology analysis
Tests 11-13: CIDR, longest-prefix routing, ARP protocol
Tests 14-15: Transport layer echo and file transfer services

---

## **COMPLIANCE WITH PROFESSOR REQUIREMENTS**

✅ **Submission 1:** Star topology, CSMA/CD, Go-Back-N, Data Link frame, Switch MAC learning, **Bridge with domain reporting**
✅ **Submission 2:** Network layer IP routing, **CIDR parsing, longest-prefix matching, ARP protocol**, routing table management
✅ **Submission 3:** Application layer, **Transport layer with ports**, **Echo service**, **File Transfer service**

All requirements met and validated through comprehensive simulation.

```
project_CN/
│
├── main.cpp
├── application/
│   ├── ApplicationLayer.h
│   ├── ApplicationLayer.cpp
│   └── ApplicationMessage.h
├── devices/
│   ├── Device.h
│   ├── Device.cpp
│   ├── EndDevice.h
│   ├── EndDevice.cpp
│   ├── Router.h
│   ├── Router.cpp
│   ├── Hub.h
│   └── Hub.cpp
│
└── network/
   ├── Frame.h
   ├── Channel.h
   ├── Channel.cpp
   ├── AckBuffer.h
   ├── AckBuffer.cpp
   ├── NetworkLayer.h
   ├── NetworkLayer.cpp
   └── NetworkPacket.h
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

//step 9 network layer and application layer
Added IP-based network packets above the existing frame layer.
Application messages are created at the application layer, wrapped into network packets, and then sent through the existing data link and physical layers.

//step 10 multi-hop routing simulation
Added a router that forwards packets between two hub segments and learns the return path for ACKs.
The routed demo uses a fresh A2/B2 pair so the original CSMA/CD and Go-Back-N tests stay independent.


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
