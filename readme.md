Important Design Choices (very important for future steps)

for compiling :
```
g++ main.cpp physical/*.cpp datalink/*.cpp network/*.cpp transport/*.cpp application/*.cpp -std=c++17 -o simulator.exe
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
  # project_CN — Network Protocols Simulator

  A compact, educational network simulator implemented in C++ that models the physical, data-link, network, transport, and application layers. The simulator demonstrates protocol behavior (CSMA/CD, Go-Back-N ARQ, ARP, simple FTP, Echo service, routing and more) and contains a suite of test scenarios in `main.cpp`.

  Purpose: provide a clear, extensible codebase for learning and experimenting with core networking protocols and layering.

  Date: May 25, 2026

  ---

  ## Quick Start

  Build (Windows / MinGW / g++):

  ```bash
  g++ main.cpp physical/*.cpp datalink/*.cpp network/*.cpp transport/*.cpp application/*.cpp -std=c++17 -O2 -o simulator.exe
  ```

  Run:

  ```powershell
  .\simulator.exe
  ```

  ---

  ## Project Overview

  This repository implements a layered network simulator. Key educational goals:

  - Demonstrate medium access control (CSMA/CD) and collision behavior.
  - Implement a reliable sliding-window ARQ (Go-Back-N) for ordered delivery.
  - Model IPv4, CIDR-based routing, and ARP resolution.
  - Provide simple application services (Echo, File Transfer) over the transport layer.

  The code is organized to make it straightforward to locate protocol implementations and extend behavior.

  ---

  ## Where Each Protocol Is Implemented

  - CSMA/CD (carrier sense, collision detection, jam, backoff): [datalink/Channel.cpp](datalink/Channel.cpp#L1) and [physical/EndDevice.cpp](physical/EndDevice.cpp#L1)
  - Go-Back-N ARQ (sender/receiver windows, cumulative ACKs, timeout retransmit): [datalink/AckBuffer.cpp](datalink/AckBuffer.cpp#L1) and [transport/TransportLayer.cpp](transport/TransportLayer.cpp#L1)
  - ARP (request/reply, cache): [network/ARP.cpp](network/ARP.cpp#L1)
  - IPv4 & CIDR utilities: [network/IPv4CIDR.cpp](network/IPv4CIDR.cpp#L1)
  - Routing & Router behavior (longest-prefix match, forwarding): [network/RoutingTable.cpp](network/RoutingTable.cpp#L1) and [network/Router.cpp](network/Router.cpp#L1)
  - Frame structure & framing helpers: [datalink/Frame.h](datalink/Frame.h#L1)
  - Switch & Bridge (MAC learning, flooding, forwarding): [datalink/Switch.cpp](datalink/Switch.cpp#L1) and [datalink/Bridge.cpp](datalink/Bridge.cpp#L1)
  - Transport segments & ports: [transport/TransportSegment.cpp](transport/TransportSegment.cpp#L1)
  - Echo service (port 7) and simple FTP-like service (port 21): [application/ApplicationServices.cpp](application/ApplicationServices.cpp#L1) and [application/ApplicationLayer.cpp](application/ApplicationLayer.cpp#L1)

  If you want line-level references to a particular function, open the file links above and search for the method names (e.g., `send()`, `handleFrame()`, `processArp()`).

  ---

  ## Directory Map (short)

  - `physical/` — device models, hubs, basic transmission logic
  - `datalink/` — frame model, channel (CSMA/CD), ack buffer (Go-Back-N), switch/bridge, topology analysis
  - `network/` — IPv4, ARP, routing table, router, packet model
  - `transport/` — transport segments and layer, port demultiplexing
  - `application/` — application messages and service handlers (Echo, FTP)
  - `main.cpp` — scenarios and test harness

  ---

  ## Implementation Timeline (completed / planned)

  All timelines below are recorded relative to this repo snapshot (May 25, 2026).

  - Week 1 — Foundations (Completed: May 04–10, 2026)
    - `physical/Device.*`, `physical/EndDevice.*`, `physical/Hub.*` — basic device models and star topology

  - Week 2 — Data Link Basics (Completed: May 11–15, 2026)
    - `datalink/Frame.h`, basic framing and parity/error detection

  - Week 3 — MAC & Switching (Completed: May 16–18, 2026)
    - `datalink/Switch.cpp`, `datalink/Bridge.cpp` — MAC learning, flooding, forwarding

  - Week 4 — Medium Access & ARQ (Completed: May 19–22, 2026)
    - CSMA/CD implemented in `datalink/Channel.cpp` and `physical/EndDevice.cpp`
    - Go-Back-N ARQ implemented in `datalink/AckBuffer.cpp` and `transport/TransportLayer.cpp`

  - Week 5 — Network Layer & ARP (Completed: May 23–24, 2026)
    - `network/IPv4CIDR.cpp`, `network/RoutingTable.cpp`, `network/Router.cpp`, `network/ARP.cpp`

  - Week 6 — Application Services & Finalization (Completed / Current: May 25, 2026)
    - `application/ApplicationServices.cpp` (Echo, FTP-like handlers)
    - Integration tests in `main.cpp`

  Planned future work (optional):

  - Add ICMP (ping) semantics and detailed timing statistics — target: June 2026
  - Enhance transport reliability to model selective-repeat ARQ or TCP-like congestion — target: June 2026

  ---

  ## How Protocols Interact (short)

  1. Applications create `ApplicationMessage` objects and submit them to the `TransportLayer`.
  2. `TransportLayer` segments data and hands `NetworkPacket` objects to the `NetworkLayer`.
  3. `NetworkLayer` performs routing/next-hop decision; ARP is used to resolve MAC addresses.
  4. `Datalink` frames packets and uses CSMA/CD on shared channels; Go-Back-N provides retransmission semantics.
  5. `Physical` devices transmit frames across links; switches/bridges forward or flood based on learned MAC tables.

  ---

  ## Quick references

  - Compile command (one-liner):

  ```bash
  g++ main.cpp physical/*.cpp datalink/*.cpp network/*.cpp transport/*.cpp application/*.cpp -std=c++17 -O2 -o simulator.exe
  ```

  - Run tests / demo:

  ```powershell
  .\simulator.exe
  ```

  ---

  ## Remaining Improvements & Notes

  - Tests and example scenarios live in `main.cpp`. If you add new services or protocols, register service handlers in `application/ApplicationServices.cpp` and add demo cases to `main.cpp`.
  - The current Go-Back-N implementation uses cumulative ACKs; consider converting to Selective Repeat if you need per-segment retransmit efficiency.
  - If you want to simulate physical media characteristics (delay, loss, bandwidth), extend `datalink/Channel.cpp` and timestamp frames at enqueue/dequeue.

  ---

  If you want, I can also:

  - Add a short Usage section showing example commands to run specific test scenarios in `main.cpp`.
  - Generate a `PROJECT_TIMELINE.md` from the timeline above with per-commit tracking.

  Would you like me to commit this README change and update the TODOs?  
| **13** | **Network** | **ARP Protocol (Submission 2)** | **✅** |
