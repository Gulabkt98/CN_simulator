# **NETWORK SIMULATOR - VISUAL ARCHITECTURE & FLOW DIAGRAMS**

**Date:** May 24, 2026  
**Document:** Comprehensive Visual Reference Guide

---

## **TABLE OF CONTENTS**

1. [System Architecture Diagram](#system-architecture-diagram)
2. [Network Topology Diagrams](#network-topology-diagrams)
3. [Complete Packet Journey Visualization](#complete-packet-journey)
4. [Layer Stack Encapsulation](#layer-stack-encapsulation)
5. [Timing Diagrams](#timing-diagrams)
6. [State Transition Diagrams](#state-transition-diagrams)

---

## **SYSTEM ARCHITECTURE DIAGRAM**

### **Complete OSI Layer Stack**

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                          NETWORK SIMULATOR ARCHITECTURE                      │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                               │
│  ┌────────────────────────────────────────────────────────────────────────┐  │
│  │  LAYER 7: APPLICATION LAYER                                           │  │
│  │  ┌──────────────────┐    ┌──────────────────┐    ┌────────────────┐  │  │
│  │  │ Echo Service     │    │ File Transfer    │    │ Application    │  │  │
│  │  │ (Port 7)         │    │ Service (Port 21)│    │ Message        │  │  │
│  │  └──────────────────┘    └──────────────────┘    └────────────────┘  │  │
│  └─────────────────────┬──────────────────────────────────────────────────┘  │
│                        │                                                      │
│  ┌─────────────────────▼──────────────────────────────────────────────────┐  │
│  │  LAYER 6-4: SESSION, PRESENTATION, TRANSPORT                          │  │
│  │  ┌────────────────────────────────────────────────────────────────┐   │  │
│  │  │ TRANSPORT LAYER                                               │   │  │
│  │  │ ├─ Port Demultiplexing (16-bit src/dst ports)              │   │  │
│  │  │ ├─ Service Handler Registry                                │   │  │
│  │  │ ├─ Sequence Numbers & Acknowledgments                      │   │  │
│  │  │ └─ TransportSegment serialization                          │   │  │
│  │  └────────────────────────────────────────────────────────────────┘   │  │
│  └─────────────────────┬──────────────────────────────────────────────────┘  │
│                        │                                                      │
│  ┌─────────────────────▼──────────────────────────────────────────────────┐  │
│  │  LAYER 3: NETWORK LAYER                                              │  │
│  │  ┌────────────────────────────────────────────────────────────────┐   │  │
│  │  │ IP Routing & Forwarding                                       │   │  │
│  │  │ ├─ IPv4 CIDR Parsing (/8 to /32 prefixes)                   │   │  │
│  │  │ ├─ Longest-Prefix Matching                                   │   │  │
│  │  │ ├─ Routing Table (dynamic routes with metrics)               │   │  │
│  │  │ ├─ TTL Decrement & Loop Prevention                           │   │  │
│  │  │ ├─ ARP Cache (IP↔MAC resolution)                            │   │  │
│  │  │ ├─ NetworkPacket serialization/deserialization              │   │  │
│  │  │ └─ Multi-hop router forwarding                              │   │  │
│  │  └────────────────────────────────────────────────────────────────┘   │  │
│  └─────────────────────┬──────────────────────────────────────────────────┘  │
│                        │                                                      │
│  ┌─────────────────────▼──────────────────────────────────────────────────┐  │
│  │  LAYER 2: DATA LINK LAYER                                            │  │
│  │  ┌────────────────────────────────────────────────────────────────┐   │  │
│  │  │ MAC Addressing & Frame Management                             │   │  │
│  │  │ ├─ Frame Structure (MAC src/dst, sequence, parity)            │   │  │
│  │  │ ├─ CSMA/CD (Carrier Sense, Collision Detection)              │   │  │
│  │  │ │   ├─ Exponential backoff (2^n retries, max 16)            │   │  │
│  │  │ │   ├─ Jam signal on collision                              │   │  │
│  │  │ │   └─ Retry management                                    │   │  │
│  │  │ ├─ Go-Back-N Flow Control                                    │   │  │
│  │  │ │   ├─ Sliding window (N=4)                                 │   │  │
│  │  │ │   ├─ Cumulative ACKs                                      │   │  │
│  │  │ │   ├─ Timeout-based retransmission                         │   │  │
│  │  │ │   └─ Duplicate ACK handling                               │   │  │
│  │  │ ├─ Switch/Bridge MAC Learning                               │   │  │
│  │  │ ├─ Flooding for unknown MACs                                │   │  │
│  │  │ └─ Parity Bit Error Detection                               │   │  │
│  │  └────────────────────────────────────────────────────────────────┘   │  │
│  └─────────────────────┬──────────────────────────────────────────────────┘  │
│                        │                                                      │
│  ┌─────────────────────▼──────────────────────────────────────────────────┐  │
│  │  LAYER 1: PHYSICAL LAYER                                             │  │
│  │  ┌────────────────────────────────────────────────────────────────┐   │  │
│  │  │ Devices & Topology                                            │   │  │
│  │  │ ├─ Device Hierarchy (Device base class)                       │   │  │
│  │  │ ├─ EndDevice (hosts with IP+MAC, network stack)              │   │  │
│  │  │ ├─ Hub (broadcast/flooding to all ports)                     │   │  │
│  │  │ ├─ Switch (MAC learning, intelligent forward)                │   │  │
│  │  │ ├─ Bridge (advanced switching with domain analysis)          │   │  │
│  │  │ ├─ Router (IP-based multi-hop routing)                       │   │  │
│  │  │ ├─ Bidirectional connections                                 │   │  │
│  │  │ ├─ Channel (CSMA/CD collision simulation)                    │   │  │
│  │  │ └─ Topology Analysis (collision & broadcast domains)         │   │  │
│  │  └────────────────────────────────────────────────────────────────┘   │  │
│  └─────────────────────────────────────────────────────────────────────────┘  │
│                                                                               │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## **NETWORK TOPOLOGY DIAGRAMS**

### **Test Topology: Multi-Subnet Network with Routing**

```
SUBNET 1 (10.0.0.0/24)                    SUBNET 2 (10.0.1.0/24)
┌──────────────────────────────────┐      ┌──────────────────────────────────┐
│                                  │      │                                  │
│  EndDevice A                     │      │  EndDevice B                     │
│  IP: 10.0.0.1                   │      │  IP: 10.0.1.2                   │
│  MAC: MAC_A                      │      │  MAC: MAC_B                      │
│    │                             │      │    │                             │
│    │              ┌──────────┐   │      │    │              ┌──────────┐   │
│    │              │  Switch  │   │      │    │              │  Switch  │   │
│    └─────┬────────┤   (SW1)  ├───┼──────┼───┬┴──────────────┤  (SW2)  ├───┘
│          │        └────┬─────┘   │      │   │               └──────────┘
│    ┌─────┴──┐           │        │      │   └──────┐
│    │ Hub H1 │           │        │      │          │
│    └────┬──┘            │        │      │          │
│         │               │        │      │          │
│    ┌────┴───────────────┼───┐    │      │    ┌─────┴───────────────┐
│    │                   │   │    │      │    │                     │
│    │ Router R1         │   │    │      │    │ Hub H2              │
│    │ IP1: 10.0.0.254   │   │    │      │    │                     │
│    │ IP2: 10.0.1.254   │   │    │      │    │                     │
│    │ MAC_R1            │   │    │      │    │                     │
│    └────────────┬──────┘   │    │      │    └─────────────────────┘
│                │           │    │      │           ▲
│                └───────────┼────┼──────┼───────────┘
│                            │    │      │
│                   (Linked)  │    │      │
│                    via    │    │      │
│                   Hub H1   │    │      │
└──────────────────────────────┘   └──────────────────────────────────┘
                                   │
                           Bi-directional Links


SUBNET 1 DEVICES:              SUBNET 2 DEVICES:
├─ A (EndDevice)              ├─ B (EndDevice)
├─ H1 (Hub)                   ├─ H2 (Hub)
├─ SW1 (Switch)               ├─ SW2 (Switch)
├─ R1 (Router - IP on both)   └─ (R1 connects both subnets)
└─ R1 acts as default gateway

ROUTING TABLES:
┌─ A's Routes:
│  ├─ 10.0.1.0/24 → via 10.0.0.254 (MAC_R1)
│  └─ 10.0.0.0/24 → Direct (local)
│
├─ R1's Routes:
│  ├─ 10.0.0.0/24 → Direct on Subnet 1
│  ├─ 10.0.1.0/24 → Direct on Subnet 2
│  └─ 0.0.0.0/0 → Drop or forward to ISP (if configured)
│
└─ B's Routes:
   ├─ 10.0.0.0/24 → via 10.0.1.254 (MAC_R1)
   └─ 10.0.1.0/24 → Direct (local)
```

### **Bridge & Topology Analysis Topology**

```
Bridge Demo Network:

┌──────────────────────┐
│   EndDevice C        │
│   IP: 10.0.0.3       │
│   MAC: MAC_C         │
│       │              │
│     [BR1]◄─ MAC_C→Port 0 (learned after C sends)
│     Bridge│            
│      MAC Table├─ MAC_C → Port 0
│             │  MAC_D → Port 1 (learned when D sends)
│       │      │
│     MAC_D────┘
│       │
│   [EndDevice D]
│   IP: 10.0.0.4
│   MAC: MAC_D


Collision Domains:
┌───────────────────────────────────────┐
│ Each device on its own collision      │
│ domain in bridge with learning        │
│                                       │
│ Domain 0: BR1 (bridge)               │
│ Domain 1: C (enddevice)              │
│ Domain 2: D (enddevice)              │
└───────────────────────────────────────┘

Broadcast Domains:
┌───────────────────────────────────────┐
│ All devices on bridge can reach       │
│ each other via flooding               │
│                                       │
│ Domain 0: {BR1, C, D}                │
└───────────────────────────────────────┘
```

---

## **COMPLETE PACKET JOURNEY**

### **Visualization: Packet from A (10.0.0.1) → B (10.0.1.2)**

```
┌─ Application Message ─┐
│ "Hello Server B"      │
└───────────┬───────────┘
            │
            ▼ (encapsulated)
┌──────────────────────────────────────────┐
│ NetworkPacket:                            │
│ ├─ sourceIP: 10.0.0.1                   │
│ ├─ destIP: 10.0.1.2                     │
│ ├─ ttl: 30                              │
│ ├─ hopCount: 0                          │
│ └─ payload: [Application Message]       │
└───────────┬──────────────────────────────┘
            │ (serialized)
            │ "NETPKT|10.0.0.1|10.0.1.2|30|0|Hello..."
            │
            ▼ (encapsulated)
┌──────────────────────────────────────────┐
│ Frame (at EndDevice A):                   │
│ ├─ sourceMAC: MAC_A                      │
│ ├─ destMAC: MAC_R1 (next hop)           │
│ ├─ payload: [NetworkPacket]              │
│ ├─ seqNum: 0 (Go-Back-N)                │
│ ├─ isACK: false                         │
│ └─ parityBit: computed                  │
└───────────┬──────────────────────────────┘
            │
            ├─ CSMA/CD CHECK
            │  ├─ Channel idle? YES
            │  ├─ Carrier sense OK? YES
            │  └─ Start transmit ✓
            │
            ▼ Physical Transmission
        ┌─────────────────┐
        │   Hub H1        │
        │  (Broadcast)    │
        └────────┬────────┘
                 │
                 ├──→ Switch (MAC_A learned)
                 ├──→ R1 receives frame ✓
                 └──→ Other devices

┌─ Router R1 Receives ─┐
│                      │
│ Check destMAC:       │
│ destMAC = MAC_R1? ✓  │
│                      │
│ Deserialize pkt:     │
│ srcIP: 10.0.0.1     │
│ dstIP: 10.0.1.2     │
│ ttl: 30             │
│                      │
│ Route lookup:        │
│ 10.0.1.2 ∈ ?        │
│ ├─ 0.0.0.0/0 ✓      │
│ ├─ 10.0.1.0/24 ✓    │
│ └─ Best: /24        │
│                      │
│ Next hop:            │
│ 10.0.1.2 (MAC_B)    │
│ Direct delivery!    │
│                      │
│ TTL: 30 → 29        │
│ hopCount: 0 → 1     │
└──────────┬───────────┘
           │
           ▼ (re-serialize)
    ┌─────────────────────────┐
    │ New Frame at Router:     │
    │ ├─ srcMAC: MAC_R1       │
    │ ├─ dstMAC: MAC_B        │
    │ ├─ payload: NETPKT(...) │
    │ │ (ttl=29, hopCount=1)  │
    │ └─ seqNum: 0            │
    └────────┬────────────────┘
             │
             ▼ Physical Transmission
         ┌─────────────────┐
         │   Hub H2        │
         │  (Broadcast)    │
         └────────┬────────┘
                  │
                  ├──→ Switch (MAC_R1 learned)
                  ├──→ B receives frame ✓
                  └──→ Other devices

┌─ EndDevice B Receives ─┐
│                        │
│ Check destMAC:        │
│ destMAC = MAC_B? ✓    │
│                        │
│ Check seqNum:         │
│ seqNum = 0?           │
│ Expected = 0? ✓       │
│ In-order! → Accept    │
│                        │
│ Send ACK(0)          │
│ Queue in AckBuffer    │
│                        │
│ Extract payload:      │
│ NETPKT|...           │
│ Deserialize:         │
│ srcIP: 10.0.0.1      │
│ dstIP: 10.0.1.2      │
│ ttl: 29              │
│ hopCount: 1          │
│                        │
│ Check: dstIP == B? ✓  │
│ → Packet for me!      │
│                        │
│ Deliver to app layer  │
│ Print message:        │
│ "[App] B received"    │
│ "[App] From: 10.0.0.1│
│ "[App] Data: Hello..." │
│                        │
│ ✓✓✓ SUCCESS ✓✓✓      │
└────────────────────────┘


ACK RETURN PATH (B → A):

AckBuffer contains: [ACK for Frame 0]
           │
           ▼ deliverACKs()
           │
    Router R1:
    ├─ destMAC = MAC_A
    ├─ Learned: MAC_A → H1
    ├─ Route ACK back to H1
    └─ Forward to Hub H1
           │
           ▼ Hub H1 broadcasts
           │
    EndDevice A receives ACK:
    ├─ isACK = true ✓
    ├─ seqNum = 0 (ACK for frame 0)
    ├─ Update window:
    │  ├─ base = 0 + 1 = 1 ✓
    │  ├─ Window slides
    │  └─ Ready to send frame 1
    │
    └─ ✓ ACK processed
```

---

## **LAYER STACK ENCAPSULATION**

### **Packet Encapsulation at Each Layer**

```
LAYER 7: APPLICATION LAYER
═══════════════════════════════════════════════════════════════
┌────────────────────────────────┐
│ ApplicationMessage              │
│ ├─ sourceIP: 10.0.0.1          │
│ ├─ destinationIP: 10.0.1.2     │
│ └─ payload: "Hello Server B"   │
└────────────┬───────────────────┘
             │
             ▼ (pass to network layer)

LAYER 3: NETWORK LAYER
═══════════════════════════════════════════════════════════════
┌──────────────────────────────────────────┐
│ NetworkPacket                             │
│ ├─ sourceIP: 10.0.0.1                   │
│ ├─ destinationIP: 10.0.1.2               │
│ ├─ ttl: 30                               │
│ ├─ hopCount: 0                           │
│ ├─ payload:                              │
│ │  └─ [ApplicationMessage content]       │
│ └─ (serialize to string)                 │
│    "NETPKT|10.0.0.1|10.0.1.2|30|0|..."  │
└──────────────┬──────────────────────────┘
               │
               ▼ (pass to data link layer)

LAYER 2: DATA LINK LAYER
═══════════════════════════════════════════════════════════════
┌──────────────────────────────────────────┐
│ Frame                                     │
│ ├─ sourceMAC: MAC_A                      │
│ ├─ destinationMAC: MAC_R1 (next hop)    │
│ ├─ payload:                              │
│ │  └─ [NetworkPacket serialized]         │
│ │     NETPKT|10.0.0.1|10.0.1.2|...      │
│ ├─ sequenceNumber: 0 (Go-Back-N)        │
│ ├─ isACK: false                         │
│ └─ parityBit: computed from payload     │
└──────────────┬──────────────────────────┘
               │
               ▼ (CSMA/CD transmission)

LAYER 1: PHYSICAL LAYER
═══════════════════════════════════════════════════════════════
┌──────────────────────────────────────────┐
│ BIT STREAM                                │
│ [MAC_A][MAC_R1][NETPKT|...][Seq][Parity]│
│ Transmitted on hub/channel                │
│ Broadcast to all connected devices        │
└──────────────────────────────────────────┘


DECAPSULATION AT DESTINATION (EndDevice B):
═══════════════════════════════════════════════════════════════

Receive BIT STREAM from Hub H2
           │
           ▼
Check destMAC = MAC_B? ✓ → Extract Frame
           │
           ▼
    ┌──────────────────┐
    │ Data Link Layer  │
    │ ├─ Extract payload
    │ ├─ Check seqNum
    │ ├─ Send ACK
    │ └─ Pass to Network Layer
    └──────────┬───────┘
               │ (payload = serialized NetworkPacket)
               │ "NETPKT|10.0.0.1|10.0.1.2|29|1|..."
               │
               ▼
    ┌──────────────────┐
    │ Network Layer    │
    │ ├─ Deserialize   │
    │ ├─ Check destIP  │
    │ ├─ Route lookup  │
    │ └─ Pass to App Layer
    └──────────┬───────┘
               │ (payload = app message data)
               │ "Hello Server B"
               │
               ▼
    ┌──────────────────┐
    │ Application Layer│
    │ ├─ Deliver message
    │ ├─ Print to console
    │ └─ ✓ RECEIVED
    └──────────────────┘
```

---

## **TIMING DIAGRAMS**

### **CSMA/CD Collision & Backoff Timeline**

```
Device A (sender)           Channel State            Backoff Counter
═════════════════════════════════════════════════════════════════════════

Time t0:
  SENSE CARRIER
  Channel idle? ─────→ ✓ IDLE
  Start transmission
  
Time t1:
  Transmitting ──────→ BUSY (A is sending)
  Frame 0 in progress
  
Time t2:
  Collision detected! ────→ COLLISION (X is sending too)
  Jam signal
  
Time t3:
  Stop transmission   ─────→ WAIT
  Retry count = 1
  Backoff = Random(0, 2^1-1) = Random(0, 1)
  Wait time = 1 slot   ─────→ BACKING OFF (1 slot)
  
Time t4:
  Wait complete
  
Time t5:
  SENSE CARRIER
  Channel idle? ─────→ ✓ IDLE
  Start transmission (retry 1)
  
Time t6:
  Transmitting ──────→ BUSY
  
Time t7:
  Another collision! ─────→ COLLISION
  Jam signal
  
Time t8:
  Stop transmission   ─────→ WAIT
  Retry count = 2
  Backoff = Random(0, 2^2-1) = Random(0, 3)
  Wait time = 2 slots  ─────→ BACKING OFF (2 slots)
  
Time t9-t10:
  Waiting...
  
Time t11:
  Wait complete
  
Time t12:
  SENSE CARRIER
  Channel idle? ─────→ ✓ IDLE
  Start transmission (retry 2)
  
Time t13:
  Transmitting ──────→ BUSY
  
Time t14:
  ✓ SUCCESS! No collision
  Frame transmitted   ─────→ IDLE
  Reset retry count = 0


EXPONENTIAL BACKOFF GRAPH:
═════════════════════════════════════════════════════════════════════════

Maximum Backoff Slots vs Retry Count:

Slots
32000 ├─ Retry 15: up to 32767
      │
 8000 ├─                      ╱─╱─╱─╱─╱
      │                    ╱─╱
  512 ├─            ╱─╱─╱
      │          ╱─╱
   64 ├─   ╱─╱─╱
      │  ╱─╱
    8 ├╱
      │
    0 └─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─ Retry #
      0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15

Retry Limit: 16 attempts
If still colliding after retry 15, frame is DROPPED
```

### **Go-Back-N Window Sliding Timeline**

```
SENDER WINDOW STATE EVOLUTION:
═════════════════════════════════════════════════════════════════════════

Initial State (base=0, next=0, window=4):
Time t0:  Window: [█ ][ ][ ][ ]  ← Ready to send
          base=0, next=0
          Action: Send Frame(0)

Time t1:  Window: [█ ][█ ][ ][ ]  ← Sent frame 0
          base=0, next=1          ← Can send more
          Action: Send Frame(1)

Time t2:  Window: [█ ][█ ][█ ][ ]  ← Sent frames 0,1
          base=0, next=2          ← Can send more
          Action: Send Frame(2)

Time t3:  Window: [█ ][█ ][█ ][█ ] ← Sent frames 0,1,2
          base=0, next=3          ← Window FULL, stop
          Action: WAIT (cannot send frame 3 yet)

Time t4:  Window: [█ ][█ ][█ ][█ ] ← Still unacknowledged
          base=0, next=3          ← Waiting for ACK
          Receive ACK(0) ✓
          Action: Move base → 1

Time t5:  Window: [ ][█ ][█ ][█ ] ← Frame 0 acknowledged
          base=1, next=3          ← Slide window, now space for frame 3
          Action: Send Frame(3)

Time t6:  Window: [ ][█ ][█ ][█ ][█ ] ← FULL AGAIN
          base=1, next=4          ← Waiting for more ACKs
          Receive ACK(1) ✓
          Receive ACK(2) ✓
          Action: Move base → 3 (cumulative ACK)

Time t7:  Window: [ ][ ][ ][█ ] ← Frames 0,1,2 acknowledged
          base=3, next=4          ← Now can send frames 4,5,6
          Action: Send Frame(4)

Time t8:  Window: [ ][ ][ ][█ ][█ ] ← Sent frames 3,4
          base=3, next=5          ← Can send more
          Action: Send Frame(5)

... (continue pattern)

TIMEOUT SCENARIO:
═════════════════════════════════════════════════════════════════════════

Time t10: Window: [█ ][█ ][█ ][█ ] ← All 4 frames sent
          base=0, next=4          ← Waiting for ACK(0)
          TIMEOUT triggered! ⏰
          Action: Retransmit ALL frames from base to next-1
                  Resend Frames 0, 1, 2, 3

          Timeline reset, retransmit happens...


RECEIVER ACK GENERATION:
═══════════════════════════════════════════════════════════════════════════

Expected=0 (waiting for seq 0)
  Receive Frame(0) ✓ ─→ Send ACK(0), expected=1
  Receive Frame(1) ✓ ─→ Send ACK(1), expected=2
  Receive Frame(3) ✗ ─→ Out of order! Send ACK(1) [last in-order]
  Receive Frame(2) ✓ ─→ Send ACK(2), expected=3
                       (cumulative ACK for 2 acknowledges 0,1,2)
  Receive Frame(4) ✓ ─→ Send ACK(4), expected=5
```

---

## **STATE TRANSITION DIAGRAMS**

### **CSMA/CD State Machine (Detailed)**

```
                    ┌─────────────────┐
                    │     INIT        │
                    │   (No Frame)    │
                    └────────┬────────┘
                             │
                      Frame ready?
                             │
                             ▼
            ┌────────────────────────────────┐
            │   CARRIER SENSE              │
            │   Check if channel is idle    │
            └─┬──────────────────────────┬──┘
              │                          │
          Busy?                      Idle?
              │                          │
              ▼                          ▼
      ┌───────────────┐      ┌─────────────────┐
      │   BACKOFF     │      │   TRANSMIT      │
      │   (Wait for   │      │  (Start sending)│
      │   random time)│      └────────┬────────┘
      └──────┬────────┘               │
             │                  Monitor for collision
             │                        │
             │                        ├─ Collision? YES
             │                        │
             │                        ▼
             │               ┌────────────────────┐
             │               │  JAM SIGNAL       │
             │               │  (Collision found)│
             │               └────┬───────────────┘
             │                    │
             │                    ├─ Increment retry count
             │                    ├─ Retry < 16?
             │                    │
             │                    ├─ YES ────────────────┐
             │                    │                      │
             │                    └─ NO (retry = 16)    │
             │                       │                  │
             │                       ▼                  ▼
             │               ┌─────────────────┐  ┌──────────────┐
             │               │ FRAME DROPPED   │  │ DROP FRAME   │
             │               │ (Give up, too   │  │ RETRIES=16   │
             │               │  many retries)  │  │ EXCEEDED     │
             │               └─────────────────┘  └──────────────┘
             │                                         │
             │        ┌────────────────────────────────┘
             │        │
             └────┬───┴──→ CARRIER SENSE (retry)
                  │
                  └────→ BACKOFF (random wait)


KEY CONSTANTS:
  - Max retries: 16
  - Backoff: Random(0, 2^retry - 1) slots
  - Retry 0: 0 slots
  - Retry 1: 0-1 slots
  - Retry 2: 0-3 slots
  - ...
  - Retry 15: 0-32767 slots
```

### **Router Forwarding State Machine**

```
                ┌──────────────────────┐
                │ RECEIVE FRAME        │
                │ (from input port)    │
                └──────────┬───────────┘
                           │
          ┌────────────────┴────────────────┐
          │                                 │
          ▼                                 ▼
    ┌────────────────────┐        ┌────────────────────┐
    │ Is destMAC my MAC? │        │ Is it broadcast?   │
    └────┬───────┬───────┘        │ (MAC_BROADCAST)    │
         │ YES   │ NO              └────┬───────┬───────┘
         │       │                      │ YES   │ NO
         │       │                      │       │
         ▼       ▼                      ▼       ▼
    ┌─────────┐  ┌─────────────────┐  (Process (Forward
     │ LOCAL  │  │   FORWARD       │   locally) to port)
     │DELIVERY│  │  (Route it)     │
     └────┬──┘   └────┬────────────┘
          │            │
          │            ├─ Check if NetworkPacket (serialized)
          │            │
          │            ├─ YES: Deserialize
          │            │  ├─ Check TTL
          │            │  │  ├─ TTL ≤ 0? → Drop (TTL exceeded)
          │            │  │  └─ TTL > 0? → Continue
          │            │  │
          │            │  ├─ Lookup route for destIP
          │            │  │  └─ Use longest-prefix matching
          │            │  │
          │            │  ├─ Found route? 
          │            │  │  ├─ YES: Get next hop MAC
          │            │  │  └─ NO: Drop (no route)
          │            │  │
          │            │  ├─ Decrement TTL
          │            │  │
          │            │  ├─ Learn reverse path
          │            │  │  └─ Store sourceIP → inPort mapping
          │            │  │
          │            │  └─ Create new Frame
          │            │     ├─ srcMAC = my MAC
          │            │     ├─ dstMAC = next hop MAC
          │            │     └─ payload = serialized packet (with new TTL)
          │            │
          │            └─ NO: Forward as plain frame
          │               ├─ Lookup destMAC in MAC table
          │               └─ Forward to port
          │
          └────────────────────────────────────┐
                                               │
                                               ▼
                                    ┌──────────────────┐
                                    │  SEND FRAME      │
                                    │  (to next hop)   │
                                    └──────────────────┘
```

---

## **SERVICE HANDLER FLOW**

### **Echo Service Processing**

```
TransportSegment arrives:
├─ SrcPort: 5000
├─ DstPort: 7 ← Echo port
├─ Payload: "Hello, Echo!"
└─ IsACK: false

           │
           ▼
    ┌──────────────────┐
    │ Port Demux       │
    │ (DstPort = 7)    │
    └────────┬─────────┘
             │
             ├─ Lookup handler for port 7
             │
             ▼
    ┌──────────────────────────────────┐
    │ EchoService::handleEchoRequest() │
    │                                  │
    │ [HANDLER LOGIC]                  │
    │ ├─ Print segment received        │
    │ ├─ Extract payload: "Hello, ..." │
    │ ├─ Print: "[Echo] Request: ..." │
    │ ├─ Print: "[Echo] Echoing back..." 
    │ └─ (In real scenario, would send│
    │    same payload back)            │
    │                                  │
    └──────────────────────────────────┘

OUTPUT EXAMPLE:
═══════════════════════════════════════════════════════════

[Transport Layer] Received segment on port 7 from 10.0.0.1:5000
[Transport Layer] Dispatching to service handler

[Echo Service] Received echo request from 10.0.0.1:5000
[Echo Service] Request: Hello, Echo!
[Echo Service] Echoing back to 10.0.0.1:5000
```

### **File Transfer Service Processing**

```
TransportSegment arrives:
├─ SrcPort: 5000
├─ DstPort: 21 ← FTP port
├─ Payload: "PUT|document.txt|This is file content"
└─ IsACK: false

           │
           ▼
    ┌──────────────────┐
    │ Port Demux       │
    │ (DstPort = 21)   │
    └────────┬─────────┘
             │
             ├─ Lookup handler for port 21
             │
             ▼
    ┌────────────────────────────────┐
    │FTPService::handleFTPRequest()  │
    │                                │
    │ [PARSER LOGIC]                 │
    │ ├─ Split by "|":              │
    │ │  [0] = "PUT"                │
    │ │  [1] = "document.txt"       │
    │ │  [2] = "This is file..."    │
    │ │                              │
    │ ├─ Command = "PUT"            │
    │ ├─ FileName = "document.txt"  │
    │ ├─ FileContent = "This is..." │
    │ │                              │
    │ ├─ Create session:            │
    │ │  key = srcIP:port:filename  │
    │ │  session.fileName = ...     │
    │ │  session.fileContent = ...  │
    │ │  session.totalSegments = 1  │
    │ │  session.receivedSegments=1 │
    │ │                              │
    │ ├─ Store in map:              │
    │ │  activeSessions[key] = ... │
    │ │                              │
    │ └─ Print: "[FTP] File stored" │
    │                                │
    └────────────────────────────────┘

OUTPUT EXAMPLE:
═══════════════════════════════════════════════════════════

[Transport Layer] Received segment on port 21 from 10.0.0.1:5000
[Transport Layer] Dispatching to service handler

[File Transfer Service] Received FTP request from 10.0.0.1:5000
[File Transfer Service] Request: PUT|document.txt|...
[File Transfer Service] PUT document.txt (46 bytes)
[File Transfer Service] File stored: document.txt
```

---

## **END-TO-END FLOW SUMMARY**

### **Complete Communication Path: A sends to B, B replies**

```
═══════════════════════════════════════════════════════════════════════════════
PHASE 1: A → B (SOURCE TO DESTINATION)
═══════════════════════════════════════════════════════════════════════════════

Layer 7: A's Application
   │
   ├─→ Create: ApplicationMessage(dst=10.0.1.2, msg="Hello B")
   │
Layer 3: A's Network
   │
   ├─→ Create: NetworkPacket(src=10.0.0.1, dst=10.0.1.2, ttl=30, payload=msg)
   ├─→ Lookup route: 10.0.1.2 ∈ 10.0.1.0/24?
   ├─→ Longest-prefix match: 10.0.1.0/24 (most specific)
   ├─→ Next hop: 10.0.0.254 (MAC_R1) via ARP cache
   │
Layer 2: A's Data Link
   │
   ├─→ CSMA/CD Phase 1: Carrier sense
   │   └─ Channel idle? YES
   │
   ├─→ Build Frame(src=MAC_A, dst=MAC_R1, payload=NetworkPacket, seq=0)
   ├─→ Go-Back-N: window [████][ ][ ][ ]
   ├─→ Add to transmit queue
   │
Layer 1: A's Physical
   │
   ├─→ Transmit on Hub H1 → Broadcast to all ports
   │
Hub H1
   │
   ├─→ Receive frame, broadcast to: Switch, Router R1, Other devices
   │
Router R1
   │
   ├─→ Check destMAC = MAC_R1? YES → Accept
   ├─→ Deserialize NetworkPacket
   ├─→ Check TTL: 30 > 0? YES
   ├─→ Route lookup: 10.0.1.2 ∈ 10.0.1.0/24? YES
   ├─→ Next hop: Direct delivery to MAC_B
   ├─→ TTL: 30 → 29
   ├─→ Learn reverse path: 10.0.0.1 → H1
   ├─→ Build new Frame(src=MAC_R1, dst=MAC_B, ttl=29)
   ├─→ Transmit on Hub H2 → Broadcast
   │
Hub H2
   │
   ├─→ Broadcast to: Switch, EndDevice B, Other devices
   │
EndDevice B
   │
Layer 1: Receive frame from Hub H2
   │
Layer 2: B's Data Link
   │
   ├─→ Check destMAC = MAC_B? YES → Accept
   ├─→ Check seqNum = 0? Expected = 0? YES → In-order
   ├─→ Send ACK(0) → Queue in AckBuffer
   ├─→ Extract payload: NetworkPacket
   │
Layer 3: B's Network
   │
   ├─→ Deserialize NetworkPacket
   ├─→ Check: destIP = 10.0.1.2? B's IP = 10.0.1.2? YES → For me!
   │
Layer 7: B's Application
   │
   └─→ Deliver message: "[App] B received from 10.0.0.1: Hello B"
       ✓✓✓ MESSAGE RECEIVED ✓✓✓


═══════════════════════════════════════════════════════════════════════════════
PHASE 2: ACK RETURN (B → A)
═══════════════════════════════════════════════════════════════════════════════

EndDevice B: Generate ACK
   │
   ├─→ Create ACK Frame: destMAC=MAC_A, seq=0, isACK=true
   ├─→ Queue in AckBuffer
   │
deliverACKs():
   │
   ├─→ Pop from AckBuffer
   ├─→ Lookup destMAC (MAC_A) in deviceDirectory
   ├─→ Route to device A
   │
Router R1 (if needed):
   │
   ├─→ Receive ACK frame
   ├─→ Look up learned path: MAC_A → H1
   ├─→ Forward to H1
   │
Hub H1:
   │
   ├─→ Broadcast ACK to: Switch, EndDevice A, Other devices
   │
EndDevice A:
   │
   ├─→ Receive ACK frame
   ├─→ Check: isACK = true? YES
   ├─→ Extract: ACK(0)
   ├─→ Update Go-Back-N window:
   │   ├─ base = 0 → base = 1
   │   ├─ Window slides: [████][ ][ ][ ] → [ ][████][ ][ ]
   │   └─ Ready to send Frame 1
   │
   └─→ ✓ ACK PROCESSED


═══════════════════════════════════════════════════════════════════════════════
PHASE 3: B SENDS REPLY (B → A)
═══════════════════════════════════════════════════════════════════════════════

(Same as Phase 1, but with reversed IPs and MACs)

EndDevice B Application:
   │
   └─→ sendApplicationData(dst=10.0.0.1, msg="Reply received...")

   (All layers repeat: App → Net → DL → PL)
   
Router R1:
   │
   ├─→ Route lookup for 10.0.0.1
   ├─→ Use learned return path
   ├─→ TTL: 30 → 29
   ├─→ Forward back to Hub H1
   │
Hub H1 → EndDevice A
   │
   └─→ ✓✓✓ REPLY MESSAGE RECEIVED AT A ✓✓✓
```

---

## **CONCLUSION**

This comprehensive visual guide demonstrates:

1. **Multi-layer encapsulation** of data from Application down to Physical layer
2. **CSMA/CD collision detection** with exponential backoff retry mechanism
3. **Go-Back-N sliding window** with cumulative ACKs and timeout-based retransmission
4. **CIDR-based longest-prefix route matching** for intelligent IP forwarding
5. **ARP protocol** for IP↔MAC address resolution
6. **Port-based service demultiplexing** in transport layer
7. **Echo and File Transfer services** at application layer
8. **Complete end-to-end communication** from source to destination and back

The simulator successfully demonstrates all OSI layers working together in a realistic network environment.

