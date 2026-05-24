# **NETWORK SIMULATOR - COMPREHENSIVE PROJECT REPORT**

**Date:** May 24, 2026  
**Project:** Computer Networks (CN) Lab - Multi-Layer Network Simulator  
**Language:** C++ (C++17)  
**Status:** ✅ COMPLETE (All Submissions 1-3)

---

## **TABLE OF CONTENTS**

1. [Executive Summary](#executive-summary)
2. [Project Architecture](#project-architecture)
3. [Detailed Layer-by-Layer Design](#detailed-layer-by-layer-design)
4. [Source-to-Destination Communication Flow](#source-to-destination-flow)
5. [Destination-to-Source (ACK/Reply) Flow](#destination-to-source-flow)
6. [Protocol State Machines](#protocol-state-machines)
7. [Service Architecture](#service-architecture)
8. [Test Results & Validation](#test-results--validation)
9. [Feature Completeness](#feature-completeness)

---

## **EXECUTIVE SUMMARY**

This project implements a **complete network stack simulator** covering all seven OSI layers (Physical through Application). It demonstrates:

- **Physical Layer:** Device hierarchy, topologies, connections
- **Data Link Layer:** CSMA/CD collision detection, Go-Back-N sliding window, MAC learning
- **Network Layer:** IP routing, CIDR, ARP, longest-prefix matching
- **Transport Layer:** Port-based demultiplexing, service registration
- **Application Layer:** Echo service, File Transfer service

The simulator validates end-to-end communication with 15 comprehensive tests across all layers.

---

## **PROJECT ARCHITECTURE**

### **High-Level System Overview**

```
┌──────────────────────────────────────────────────────────────┐
│                    NETWORK SIMULATOR                          │
├──────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌─────────────────────────────────────────────────────┐    │
│  │         APPLICATION LAYER (Services)                │    │
│  │  ┌──────────────┐  ┌──────────────────────────┐    │    │
│  │  │ Echo Service │  │ File Transfer Service    │    │    │
│  │  │  (Port 7)    │  │      (Port 21)           │    │    │
│  │  └──────────────┘  └──────────────────────────┘    │    │
│  └──────────────────┬───────────────────────────────┘    │
│                     │                                      │
│  ┌──────────────────▼───────────────────────────────┐    │
│  │      TRANSPORT LAYER (Port Demultiplexing)       │    │
│  │  ┌─ Port 7  → Echo Handler                       │    │
│  │  │                                                │    │
│  │  └─ Port 21 → FTP Handler                        │    │
│  └──────────────────┬───────────────────────────────┘    │
│                     │                                      │
│  ┌──────────────────▼───────────────────────────────┐    │
│  │  NETWORK LAYER (IP Routing, ARP, CIDR)           │    │
│  │  ┌────────────────────────────────────────────┐  │    │
│  │  │ Router A, Router B, CIDR Routing Tables    │  │    │
│  │  │ Longest-Prefix Match, ARP Cache            │  │    │
│  │  └────────────────────────────────────────────┘  │    │
│  └──────────────────┬───────────────────────────────┘    │
│                     │                                      │
│  ┌──────────────────▼───────────────────────────────┐    │
│  │ DATA LINK LAYER (MAC, CSMA/CD, Go-Back-N)        │    │
│  │  ┌────────────────────────────────────────────┐  │    │
│  │  │ CSMA/CD: Carrier Sense, Collision Detect   │  │    │
│  │  │ Go-Back-N: Sliding Window, ACK, Retransmit │  │    │
│  │  │ Switch/Bridge: MAC Learning, Flooding      │  │    │
│  │  └────────────────────────────────────────────┘  │    │
│  └──────────────────┬───────────────────────────────┘    │
│                     │                                      │
│  ┌──────────────────▼───────────────────────────────┐    │
│  │   PHYSICAL LAYER (Devices & Topology)           │    │
│  │  ┌────────────────────────────────────────────┐  │    │
│  │  │ Devices: EndDevice, Hub, Switch, Bridge    │  │    │
│  │  │ Topology: Star (multiple hubs)             │  │    │
│  │  │ Connections: Bidirectional links           │  │    │
│  │  └────────────────────────────────────────────┘  │    │
│  └─────────────────────────────────────────────────┘    │
│                                                               │
└──────────────────────────────────────────────────────────────┘
```

---

## **DETAILED LAYER-BY-LAYER DESIGN**

### **LAYER 1: PHYSICAL LAYER**

**Components:**
- `Device` (base class) - Abstract network device
- `EndDevice` - Host with MAC address, IP address, network stack
- `Hub` - Broadcast device (floods to all ports)
- `Switch` - MAC-learning device (intelligent forwarding)
- `Bridge` - Advanced switching with domain analysis
- `Router` - Multi-hop packet forwarding with IP routing

**Topology:**
```
EndDevice A                EndDevice B
    |                           |
    └──────┬──────┬─────────────┘
           │      │
        [Hub H1]──┤
                  │
              ┌───┴────────┐
              │            │
          [Switch]    [Bridge]
              │
    EndDevice C, D, E
```

**Physical Connections:**
- Each device has a `connections[]` vector
- Devices can be connected: Device A ↔ Device B
- All devices receive frames and can forward

---

### **LAYER 2: DATA LINK LAYER**

#### **2.1 Frame Structure**

```
┌────────────────────────────────────────┐
│        FRAME FORMAT                    │
├────────────────────────────────────────┤
│ Source MAC      (string)               │  "MAC_A"
├────────────────────────────────────────┤
│ Destination MAC (string)               │  "MAC_B"
├────────────────────────────────────────┤
│ Payload         (string)               │  Data or NetworkPacket
├────────────────────────────────────────┤
│ Sequence Number (int)                  │  0-7 (window size=4)
├────────────────────────────────────────┤
│ Is ACK Flag     (bool)                 │  true/false
├────────────────────────────────────────┤
│ Parity Bit      (int)                  │  0 or 1 (error detection)
└────────────────────────────────────────┘
```

#### **2.2 CSMA/CD Protocol**

**State Diagram:**

```
START
  │
  ▼
┌──────────────────────┐
│ Sense Carrier        │  ← Check if channel is idle
│ (Channel idle?)      │
└──────┬───────────────┘
       │ NO (busy)
       ▼
    WAIT (backoff)   ← Exponential backoff
       │
       └──→ Retry (up to 16 times)
  
       │ YES (idle)
       ▼
┌──────────────────────┐
│ Transmit Frame       │
└──────┬───────────────┘
       │
       ▼
┌──────────────────────┐
│ Collision?           │  ← Monitor during transmission
└──────┬───────────────┘
       │
       ├─ NO ──→ SUCCESS ✓
       │
       └─ YES ──→ JAM SIGNAL → BACKOFF → RETRY
                  (exponential delay)
```

**Backoff Algorithm:**
- Retry count: 0-15
- Backoff time: Random(0, 2^retryCount - 1) × slot_time
- Max retries: 16 → Frame dropped

**Example:**
```
Retry 0: Backoff = Random(0, 0) = 0 slots
Retry 1: Backoff = Random(0, 1) = 0-1 slots
Retry 2: Backoff = Random(0, 3) = 0-3 slots
Retry 3: Backoff = Random(0, 7) = 0-7 slots
...
Retry 15: Backoff = Random(0, 32767) slots
```

#### **2.3 Go-Back-N Flow Control**

**Sliding Window Mechanism:**

```
SENDER SIDE:
═══════════════════════════════════════════════════════════

Window Size (N) = 4

Frames:     0    1    2    3    4    5    6    7
           [████████████]  ← Unacknowledged frames (can transmit)
           └─┬──────────┴─ Base
              └─ Next (points to next unsent)

Base = 0, Next = 3 (sent frames 0,1,2; ready to send 3)

State:
 - Base: Oldest unacknowledged frame
 - Next: Next frame to send
 - Constraint: Next - Base ≤ N

Actions:
 ├─ Send Frame: Increment Next (if window not full)
 ├─ Receive ACK(k): Move Base to k+1 (slide window)
 └─ Timeout: Retransmit all frames from Base to Next-1


RECEIVER SIDE:
═══════════════════════════════════════════════════════════

Expected Sequence: 2 (waiting for frame 2)

Received Frame:
 ├─ Seq = 2   → IN ORDER ✓ → Accept & Send ACK(2) → Advance Expected
 ├─ Seq = 5   → OUT OF ORDER → Send ACK(1) [last in-order]
 └─ Seq = 1   → DUPLICATE → Send ACK(1)

Window only accepts frame with seq == Expected.

ACK(k) means: "All frames up to k received successfully; ready for k+1"
```

**Example: Send 4 Frames → Wait for ACKs**

```
Time  Sender              Receiver            Channel
────────────────────────────────────────────────────────────
t1    Send [Frame 0]  ──────────────────→  Recv [Frame 0]
                                          Send [ACK 0]  ──→
t2    Send [Frame 1]  ──────────────────→  Recv [Frame 1]
                                          Send [ACK 1]  ──→
t3    Send [Frame 2]  ──────────────────→  Recv [Frame 2]
                                          Send [ACK 2]  ──→
t4    Send [Frame 3]  ──────────────────→  Recv [Frame 3]
                                          Send [ACK 3]  ──→

t5    Recv [ACK 0]←──  Base = 1
      Recv [ACK 1]←──  Base = 2
      Recv [ACK 2]←──  Base = 3
      Recv [ACK 3]←──  Base = 4 (All ACK'd, window slides)

      Window now free to send [Frame 4]
```

#### **2.4 Switch MAC Learning & Forwarding**

**MAC Table Evolution:**

```
Initial State:
┌──────────────────┐
│ MAC Table: EMPTY │
└──────────────────┘

Step 1: Frame arrives from MAC_A on Port 0
Device A ──→ [Frame from MAC_A]
               ↓
          [Switch MAC Table]
          ┌─────────────────┐
          │ MAC_A → Port 0  │  ← Learned!
          └─────────────────┘

Step 2: Frame to MAC_A arrives on Port 1
Device B ──→ [Frame to MAC_A]
               ↓
          [Switch Lookup]
          ├─ Dest MAC: MAC_A
          ├─ Table lookup: MAC_A → Port 0
          ├─ Action: Unicast to Port 0 ✓
          └─ Device A receives frame
```

**Forwarding Decision Tree:**

```
RECEIVE Frame on Port i
    │
    ├─→ Learn: SourceMAC → Port i
    │
    ├─→ Lookup DestinationMAC in table
    │
    ├─ FOUND (in table)
    │   └─→ Unicast to Port (don't send back on Port i)
    │
    └─ NOT FOUND (unknown MAC)
        └─→ Flood to all ports EXCEPT Port i
            (broadcast/unknown destination)
```

---

### **LAYER 3: NETWORK LAYER**

#### **3.1 IPv4 CIDR Addressing**

**CIDR Notation:**
```
Format: X.X.X.X/Y

where:
 X.X.X.X = IP Address (dotted decimal)
 Y = Prefix Length (8-32 bits)

Examples:
 10.0.0.0/24  → Network with 24-bit prefix (256 hosts, /24)
 192.168.0.0/16 → 16-bit prefix (65536 hosts)
 172.16.0.0/12 → 12-bit prefix (1048576 hosts)
```

**Subnet Mask Calculation:**

```
/24 Prefix:
Bits:   11111111 11111111 11111111 00000000
Mask:   255      255      255      0

/25 Prefix:
Bits:   11111111 11111111 11111111 10000000
Mask:   255      255      255      128
```

**Network Membership Test:**

```
IP: 10.0.0.50
CIDR: 10.0.0.0/25

Step 1: Get mask for /25
Mask = 255.255.255.128 (binary: ...10000000)

Step 2: AND operation
IP & Mask       = 10.0.0.50 & 255.255.255.128
              = (0x0A,0x00,0x00,0x32) & (0xFF,0xFF,0xFF,0x80)
              = (0x0A,0x00,0x00,0x00)
              = 10.0.0.0

Network Addr    = 10.0.0.0

Step 3: Compare
Network (10.0.0.0) == Network (10.0.0.0) ? YES ✓ MEMBER

Therefore: 10.0.0.50 ∈ 10.0.0.0/25 ✓
```

#### **3.2 Longest-Prefix Route Matching**

**Routing Table:**

```
Destination       | Next Hop    | Prefix | Priority
─────────────────────────────────────────────────
0.0.0.0/0         | 10.0.0.254  | 0      | Lowest  (default route)
10.0.0.0/24       | 10.0.0.1    | 24     | Medium
10.0.0.0/25       | 10.0.0.2    | 25     | Highest (most specific)
```

**Route Lookup Example:**

```
Looking up: 10.0.0.50

Matching Routes:
 ├─ 0.0.0.0/0      matches? YES (default always matches) ✓
 ├─ 10.0.0.0/24    matches? YES (50 in 0-255) ✓
 └─ 10.0.0.0/25    matches? YES (50 in 0-127) ✓

Best Match: 10.0.0.0/25 (prefix /25 is most specific)
            ↓
Next Hop: 10.0.0.2 (MAC: MAC_B)

═════════════════════════════════════════════════
Forward packet to 10.0.0.2 with destination MAC MAC_B
```

**Another Example:**

```
Looking up: 192.168.1.100

Matching Routes:
 ├─ 0.0.0.0/0      matches? YES ✓
 ├─ 10.0.0.0/24    matches? NO (192.168.1.0 ≠ 10.0.0.0)
 └─ 10.0.0.0/25    matches? NO

Best Match: 0.0.0.0/0 (default route, only match)
            ↓
Next Hop: 10.0.0.254 (MAC: MAC_GW)

═════════════════════════════════════════════════
Forward packet to default gateway 10.0.0.254
```

#### **3.3 ARP (Address Resolution Protocol)**

**ARP Cache:**

```
┌──────────────────────────────────────┐
│         ARP CACHE                    │
├────────────────────┬─────────────────┤
│ IP Address         │ MAC Address     │
├────────────────────┼─────────────────┤
│ 10.0.0.1           │ MAC_A           │
│ 10.0.0.2           │ MAC_B           │
│ 10.0.0.254         │ MAC_ROUTER      │
└────────────────────┴─────────────────┘
```

**ARP Message Structure:**

```
ARP REQUEST:
┌──────────────────────────────────┐
│ Type: REQUEST                    │
│ Sender IP: 10.0.0.1              │
│ Sender MAC: MAC_A                │
│ Target IP: 10.0.0.2              │
│ Target MAC: (empty - asking)     │
└──────────────────────────────────┘

ARP REPLY:
┌──────────────────────────────────┐
│ Type: REPLY                      │
│ Sender IP: 10.0.0.2              │
│ Sender MAC: MAC_B                │
│ Target IP: 10.0.0.1              │
│ Target MAC: MAC_A                │
└──────────────────────────────────┘
```

#### **3.4 Network Packet Structure**

```
┌────────────────────────────────────────┐
│        NETWORK PACKET (IP Layer)       │
├────────────────────────────────────────┤
│ Source IP       (string)               │  "10.0.0.1"
├────────────────────────────────────────┤
│ Destination IP  (string)               │  "10.0.0.2"
├────────────────────────────────────────┤
│ TTL (Time To Live) (int)               │  30 (decrements on each hop)
├────────────────────────────────────────┤
│ Hop Count       (int)                  │  0 (increments on each hop)
├────────────────────────────────────────┤
│ Payload         (string)               │  Transport segment or app data
└────────────────────────────────────────┘

TTL: Prevents infinite loops
 - Initial value: 30
 - Decremented at each router
 - When TTL = 0: packet dropped (loop prevention)
```

#### **3.5 Router Operation**

**Route Lookup & Forwarding:**

```
Router R1 receives Frame with NetworkPacket
    │
    ├─→ Deserialize NetworkPacket
    │
    ├─→ Check TTL
    │   ├─ TTL ≤ 0 → Drop packet (TTL exceeded)
    │   └─ TTL > 0 → Continue
    │
    ├─→ Lookup route for DestIP
    │   └─ Use longest-prefix matching
    │       └─ Get next hop MAC
    │
    ├─→ Decrement TTL
    │
    ├─→ Create new Frame with:
    │   ├─ SourceMAC = RouterMAC
    │   ├─ DestinationMAC = NextHopMAC
    │   └─ Payload = NetworkPacket (serialized)
    │
    ├─→ Learn return path for ACKs
    │   └─ Store: DestIP → NextHop mapping
    │
    └─→ Forward Frame to NextHop device
```

---

### **LAYER 4: TRANSPORT LAYER**

#### **4.1 Transport Segment Structure**

```
┌─────────────────────────────────────────┐
│     TRANSPORT SEGMENT                   │
├─────────────────────────────────────────┤
│ Source Port     (uint16)                │  5000
├─────────────────────────────────────────┤
│ Destination Port (uint16)               │  7 (Echo) or 21 (FTP)
├─────────────────────────────────────────┤
│ Sequence Number (uint16)                │  0-65535
├─────────────────────────────────────────┤
│ Acknowledgment #  (uint16)              │  0 (in request) or seqNum (in reply)
├─────────────────────────────────────────┤
│ Has ACK Flag    (bool)                  │  false (request) or true (reply)
├─────────────────────────────────────────┤
│ Payload         (string)                │  Application data
└─────────────────────────────────────────┘
```

#### **4.2 Port-Based Service Demultiplexing**

**Service Registration:**

```
Transport Layer: Host B (Port 5001)
┌──────────────────────────────────────┐
│ Service Handlers:                    │
├──────────────────────────────────────┤
│ Port 7  → EchoService::handler()     │
│ Port 21 → FTPService::handler()      │
│ Port 23 → TelnetService (if added)   │
│ ...                                  │
└──────────────────────────────────────┘
```

**Incoming Segment Processing:**

```
Segment arrives with DestPort = 7
         │
         ▼
    ┌─────────────┐
    │ Demux:      │
    │ Port 7      │ ← Lookup handler
    └─────┬───────┘
          │
          ▼
    ┌──────────────────────┐
    │ Found handler for    │
    │ EchoService          │
    └──────┬───────────────┘
           │
           ▼
    ┌──────────────────────────┐
    │ Call:                    │
    │ EchoService::handler()   │
    │ (segment, srcIP, srcPort)│
    └──────────────────────────┘
```

---

### **LAYER 5-7: APPLICATION LAYER**

#### **5.1 Echo Service (Port 7)**

**Protocol:**

```
Echo Client Request:
┌──────────────────────────────────────┐
│ TransportSegment                     │
├──────────────────────────────────────┤
│ SrcPort: 5000 (client port)          │
│ DstPort: 7 (echo port)               │
│ SeqNum: 1                            │
│ Payload: "Hello, Echo!"              │
│ IsACK: false                         │
└──────────────────────────────────────┘
        │
        ▼ (transmission)
    [Router Network]
        ▼
Echo Server Handler:
│ Receives message
│ → Print "[Echo Service] Echoing back..."
│ → Send same payload back to client
```

**Echo Flow Diagram:**

```
Client A                        Server B
(10.0.0.1)                      (10.0.0.2)
    │                               │
    │─ TransportSegment(          │
    │   Port5000→7,               │
    │   "Hello, Echo!")           │
    │─────────────────────────────→
    │                               │
    │                         (Echo received)
    │                         (Payload printed)
    │                               │
    │                        Send Reply:
    │                        Port 7→5000
    │←──────────────────────────────
    │   (same data echoed back)
    │
(Echo received and printed)
```

#### **5.2 File Transfer Service (Port 21)**

**Protocol:**

```
PUT Command:
┌──────────────────────────────────────────────┐
│ Format: PUT|filename|content                │
│                                              │
│ Example:                                    │
│ "PUT|document.txt|This is file content"    │
└──────────────────────────────────────────────┘

GET Command:
┌──────────────────────────────────────────────┐
│ Format: GET|filename                        │
│                                              │
│ Example:                                    │
│ "GET|document.txt"                         │
└──────────────────────────────────────────────┘
```

**FTP Flow:**

```
Client A                        Server B
(10.0.0.1)                      (10.0.0.2)
    │                               │
    │─ PUT|file.txt|content        │
    │─────────────────────────────→
    │   (Port 5000→21)              │
    │                               │
    │                         (File stored)
    │                         Create session:
    │                         fileName: file.txt
    │                         content: ...
    │                               │
    │                         Send ACK:
    │←──────────────────────────────
    │   "FILE_STORED"
    │
(File transfer complete)
```

---

## **SOURCE-TO-DESTINATION FLOW**

### **Complete End-to-End Communication Example**

**Scenario:**
- Client A (IP: 10.0.0.1, MAC: MAC_A) sends "Hello" to Server B (IP: 10.0.0.2, MAC: MAC_B)
- They're on different subnets separated by Router R1

**Network Topology:**

```
Subnet 1                    Subnet 2
(10.0.0.0/24)          (10.0.1.0/24)

    [A] ──┐                  ┌── [B]
          │                  │
    [Hub1]────[R1]────[Hub2]
          │       ↑          │
          │       │          │
       [Switch]  (Routes)  [Switch]
    
A's IP: 10.0.0.1    R1's IP: 10.0.0.254 (on Subnet 1)
A's MAC: MAC_A           and: 10.0.1.254 (on Subnet 2)
B's IP: 10.0.1.2
B's MAC: MAC_B

Routing Tables:
A: 10.0.1.0/24 → NextHop: 10.0.0.254 (MAC_R1)
R1: 10.0.1.0/24 → NextHop: 10.0.1.2 (MAC_B directly)
B: 10.0.0.0/24 → NextHop: 10.0.0.254 (MAC_R1)
```

### **STEP-BY-STEP PACKET JOURNEY (A → B)**

```
═══════════════════════════════════════════════════════════════════════════════
TIME 1: APPLICATION LAYER - Message Composition at A
═══════════════════════════════════════════════════════════════════════════════

Host A Application Layer:
┌────────────────────────────────────┐
│ sendApplicationData(                │
│   destIP: "10.0.1.2",              │
│   data: "Hello Server B"           │
│ )                                  │
└────────────────────────────────────┘
                │
                ▼
┌────────────────────────────────────────┐
│ ApplicationMessage:                     │
│ ├─ sourceIP: 10.0.0.1                 │
│ ├─ destinationIP: 10.0.1.2            │
│ ├─ payload: "Hello Server B"          │
│ └─ timestamp: [sys time]              │
└────────────────────────────────────────┘

Action: Pass to NetworkLayer::sendPacket()


═══════════════════════════════════════════════════════════════════════════════
TIME 2: NETWORK LAYER - Packet Formation at A
═══════════════════════════════════════════════════════════════════════════════

NetworkLayer::sendPacket()
    │
    ├─→ Create NetworkPacket:
    │   ├─ sourceIP: 10.0.0.1
    │   ├─ destinationIP: 10.0.1.2
    │   ├─ ttl: 30
    │   ├─ hopCount: 0
    │   └─ payload: "Hello Server B"
    │
    ├─→ Lookup route for 10.0.1.2:
    │   ├─ Check: 10.0.1.2 ∈ 10.0.1.0/24 ✓ (after longest-prefix match)
    │   ├─ Next hop IP: 10.0.0.254 (Router R1)
    │   ├─ Next hop MAC: MAC_R1 (via ARP cache)
    │   └─ Route found! → MAC_R1
    │
    ├─→ Serialize NetworkPacket to string:
    │   └─ "NETPKT|10.0.0.1|10.0.1.2|30|0|Hello Server B"
    │
    └─→ Pass to DataLinkLayer::send()

┌─────────────────────────────────────────────────┐
│ NetworkPacket (serialized):                      │
│ "NETPKT|10.0.0.1|10.0.1.2|30|0|Hello Server B" │
└─────────────────────────────────────────────────┘


═══════════════════════════════════════════════════════════════════════════════
TIME 3: DATA LINK LAYER - Frame Creation at A (with CSMA/CD)
═══════════════════════════════════════════════════════════════════════════════

EndDevice A::send(payload, destMAC)
    │
    ├─→ CSMA/CD PHASE 1: CARRIER SENSE
    │   ├─ Check: Is channel idle?
    │   ├─ Channel::busy? NO → Continue
    │   └─ Channel::activeSender? NO → Continue
    │
    ├─→ CSMA/CD PHASE 2: TRANSMIT
    │   ├─ Mark channel as busy
    │   ├─ Build Frame:
    │   │   ├─ sourceMAC: MAC_A
    │   │   ├─ destMAC: MAC_R1 (next hop)
    │   │   ├─ payload: "NETPKT|10.0.0.1|10.0.1.2|30|0|Hello Server B"
    │   │   ├─ seqNum: 0 (from window base)
    │   │   ├─ isACK: false
    │   │   └─ parityBit: computed from payload bits
    │   │
    │   └─ Add to transmit queue (Go-Back-N window)
    │       ├─ base = 0, next = 1
    │       ├─ Unack'd frames: [Frame0]
    │       └─ Window state: [████][ ][ ][ ]
    │
    └─→ Call Channel::transmit()

┌──────────────────────────────────────────────┐
│ Frame FORMAT:                                │
│ [MAC_A][MAC_R1]                             │
│ [NETPKT|10.0.0.1|10.0.1.2|30|0|Hello...]   │
│ [Seq=0][ACK=false][Parity=0]               │
└──────────────────────────────────────────────┘


═══════════════════════════════════════════════════════════════════════════════
TIME 4: CSMA/CD COLLISION DETECTION (Simulated Check)
═══════════════════════════════════════════════════════════════════════════════

Check for collision:
    ├─ activeSender == A? YES (A is sending)
    ├─ Collision detected? NO
    └─ Continue transmission ✓


═══════════════════════════════════════════════════════════════════════════════
TIME 5: PHYSICAL LAYER - Transmission on Hub H1
═══════════════════════════════════════════════════════════════════════════════

Hub H1::receive(frame, sender=A)
    │
    ├─→ Broadcast to all connected devices EXCEPT sender
    │   ├─ Send to Switch: [MAC_A][MAC_R1][payload...]
    │   └─ Send to other EndDevices: [MAC_A][MAC_R1][payload...]
    │
    └─→ Router R1 receives frame


═══════════════════════════════════════════════════════════════════════════════
TIME 6: ROUTER - PACKET FORWARDING (Network Layer)
═══════════════════════════════════════════════════════════════════════════════

Router R1::receive(frame)
    │
    ├─→ Check if destMAC == R1's MAC? YES
    │   └─→ Accept frame (destined for me)
    │
    ├─→ Deserialize NetworkPacket:
    │   ├─ sourceIP: 10.0.0.1
    │   ├─ destinationIP: 10.0.1.2
    │   ├─ ttl: 30
    │   └─ payload: "Hello Server B"
    │
    ├─→ Check TTL:
    │   ├─ TTL > 0? YES (30 > 0) ✓
    │   └─ Continue
    │
    ├─→ Lookup route for 10.0.1.2:
    │   ├─ Check routing table:
    │   │   ├─ 0.0.0.0/0: matches? YES (default)
    │   │   ├─ 10.0.1.0/24: matches? YES ← BEST (most specific!)
    │   │   └─ Prefix /24 is most specific
    │   │
    │   ├─ Result: 10.0.1.0/24 → NextHop: 10.0.1.2 (MAC_B)
    │   └─ Direct delivery to B
    │
    ├─→ Decrement TTL:
    │   ├─ Old TTL: 30
    │   ├─ New TTL: 29 (send forward with 29)
    │   └─ hopCount: 1
    │
    ├─→ Create new Frame:
    │   ├─ sourceMAC: MAC_R1 (router's MAC on Subnet 2)
    │   ├─ destMAC: MAC_B (direct delivery)
    │   ├─ payload: "NETPKT|10.0.0.1|10.0.1.2|29|1|Hello..."
    │   └─ seqNum: 0
    │
    ├─→ Learn reverse path for ACKs:
    │   ├─ Store: 10.0.0.1 → A (via H1, MAC_A)
    │   └─ Future ACKs from B to A routed back through H1
    │
    └─→ Transmit Frame to Hub H2

┌──────────────────────────────────────┐
│ New Frame (after routing):            │
│ [MAC_R1][MAC_B]                      │
│ [NETPKT|...|10.0.1.2|29|1|Hello...] │
│ [TTL decremented: 30→29]             │
└──────────────────────────────────────┘


═══════════════════════════════════════════════════════════════════════════════
TIME 7: PHYSICAL LAYER - Transmission on Hub H2
═══════════════════════════════════════════════════════════════════════════════

Hub H2::receive(frame, sender=R1)
    │
    ├─→ Broadcast frame to all ports EXCEPT R1
    │   ├─ Send to B: [MAC_R1][MAC_B][NETPKT|...]
    │   └─ B's interface receives frame
    │
    └─→ EndDevice B receives frame


═══════════════════════════════════════════════════════════════════════════════
TIME 8: DESTINATION - FRAME ACCEPTANCE AT B
═══════════════════════════════════════════════════════════════════════════════

EndDevice B::receive(frame)
    │
    ├─→ Check: destMAC == B's MAC? YES
    │   └─→ Frame destined for me ✓
    │
    ├─→ Extract payload: "NETPKT|10.0.0.1|10.0.1.2|29|1|Hello..."
    │
    ├─→ Check parity bit (error detection):
    │   ├─ Compute parity of payload
    │   ├─ parity == frame.parityBit? YES ✓
    │   └─ No errors detected
    │
    ├─→ Check seqNum:
    │   ├─ Frame seqNum: 0
    │   ├─ Expected seqNum: 0
    │   ├─ Match? YES ✓ → In-order frame
    │   └─ Advance expected to 1
    │
    ├─→ Send ACK(0):
    │   └─ Create ACK frame:
    │       ├─ destMAC: MAC_A (sender's MAC)
    │       ├─ seqNum: 0 (acknowledging frame 0)
    │       ├─ isACK: true
    │       └─ Queue in AckBuffer
    │
    └─→ Deserialize NetworkPacket from payload:
        └─ Extract: 10.0.0.1 → 10.0.1.2, data: "Hello..."


═══════════════════════════════════════════════════════════════════════════════
TIME 9: NETWORK LAYER - PACKET PROCESSING AT B
═══════════════════════════════════════════════════════════════════════════════

EndDevice B::receive() [Network Layer]
    │
    ├─→ Is this packet for me?
    │   ├─ destIP: 10.0.1.2
    │   ├─ B's IP: 10.0.1.2
    │   ├─ Match? YES ✓
    │   └─ This packet is destined for me
    │
    └─→ Pass to Application Layer


═══════════════════════════════════════════════════════════════════════════════
TIME 10: APPLICATION LAYER - MESSAGE DELIVERY AT B
═══════════════════════════════════════════════════════════════════════════════

ApplicationLayer B::deliverMessage()
    │
    ├─→ Create ApplicationMessage:
    │   ├─ sourceIP: 10.0.0.1
    │   ├─ destinationIP: 10.0.1.2
    │   └─ payload: "Hello Server B"
    │
    ├─→ Print to console:
    │   └─ "[Application Layer] host B received message"
    │   └─ "[Application Layer] From: 10.0.0.1"
    │   └─ "[Application Layer] Data: Hello Server B"
    │
    └─→ MESSAGE RECEIVED ✓✓✓

    ┌─────────────────────────────────────────────────────────┐
    │ ✓✓✓ APPLICATION MESSAGE DELIVERED SUCCESSFULLY ✓✓✓   │
    │                                                          │
    │ Source: A (10.0.0.1)                                    │
    │ Destination: B (10.0.1.2)                               │
    │ Message: "Hello Server B"                               │
    │ Hops: 1 (via Router R1)                                 │
    └─────────────────────────────────────────────────────────┘


═══════════════════════════════════════════════════════════════════════════════
SOURCE → DESTINATION FLOW SUMMARY
═══════════════════════════════════════════════════════════════════════════════

A's Application
    ↓
A's NetworkLayer (IP, routing lookup, CIDR match)
    ↓
A's DataLinkLayer (Frame creation, CSMA/CD, Go-Back-N)
    ↓
A's Physical (Send on Hub H1)
    ↓
Hub H1 (Broadcast)
    ↓
Router R1 (Receive, deserialize, route lookup, TTL decrement, serialize)
    ↓
Router R1's Physical (Send on Hub H2)
    ↓
Hub H2 (Broadcast)
    ↓
B's Physical (Receive frame)
    ↓
B's DataLinkLayer (Check seqNum, send ACK, extract payload)
    ↓
B's NetworkLayer (Extract IP packet, check destination)
    ↓
B's Application (Deliver message, print)
    ↓
✓ MESSAGE ARRIVES AT DESTINATION
```

---

## **DESTINATION-TO-SOURCE FLOW**

### **ACK & REPLY Path (B → A)**

```
═══════════════════════════════════════════════════════════════════════════════
TIME 11: ACK GENERATION AT B (Going Back to A)
═══════════════════════════════════════════════════════════════════════════════

EndDevice B [from TIME 8]:
    │
    ├─→ Queued ACK for frame 0:
    │   ├─ destMAC: MAC_A (sender of original frame)
    │   ├─ seqNum: 0
    │   └─ isACK: true
    │
    └─→ ACK is stored in AckBuffer queue

┌──────────────────────────────────────────┐
│ ACK Frame:                                │
│ [MAC_B][MAC_A]  ← Note: reversed MACs   │
│ [SeqNum=0][IsACK=true]                  │
│ [No payload needed]                     │
└──────────────────────────────────────────┘

ACTION: deliverACKs() at application


═══════════════════════════════════════════════════════════════════════════════
TIME 12: NETWORK LAYER - ROUTE ACK BACK TO A
═══════════════════════════════════════════════════════════════════════════════

deliverACKs(deviceDirectory, fallbackReceiver)
    │
    ├─→ Pop ACK from AckBuffer
    │   └─ ACK frame: [MAC_B][MAC_A][Seq=0][IsACK]
    │
    ├─→ Lookup destination MAC in deviceDirectory:
    │   ├─ Find MAC_A in device map
    │   ├─ Router learned: MAC_A → Via H1 when sending initial packet
    │   └─ Send ACK to A
    │
    └─→ Call A.receive(ACKframe, B)


═══════════════════════════════════════════════════════════════════════════════
TIME 13: ROUTER R1 - PROCESSES ACK
═══════════════════════════════════════════════════════════════════════════════

Router R1::receive(frame from B)
    │
    ├─→ Check destMAC: MAC_A (not R1)
    │   └─→ This is a routed ACK frame
    │
    ├─→ Deserialize if NetworkPacket?
    │   └─ This is a plain ACK frame (no IP packet inside)
    │   └─ Just forward based on MAC
    │
    ├─→ Route lookup based on destination MAC:
    │   ├─ Reverse path learned: MAC_A → H1
    │   └─ Forward ACK to H1
    │
    └─→ Send frame to H1


═══════════════════════════════════════════════════════════════════════════════
TIME 14: HUB H1 BROADCASTS ACK
═══════════════════════════════════════════════════════════════════════════════

Hub H1::receive(frame)
    │
    ├─→ Broadcast to all ports except sender (R1)
    │   └─ Send ACK to A: [MAC_B][MAC_A][Seq=0][IsACK]
    │
    └─→ A receives ACK frame


═══════════════════════════════════════════════════════════════════════════════
TIME 15: SENDER A - ACK RECEPTION
═══════════════════════════════════════════════════════════════════════════════

EndDevice A::receive(ACK frame)
    │
    ├─→ Check: isACK flag? YES
    │   └─→ This is an acknowledgment
    │
    ├─→ Extract ACK seqNum: 0
    │   └─→ ACK(0) means: "Frames up to 0 received successfully"
    │
    ├─→ Update Go-Back-N window:
    │   ├─ Old state: base=0, next=1, unack'd=[Frame0]
    │   ├─ Process ACK(0): Move base = 0 + 1 = 1
    │   ├─ New state: base=1, next=1, unack'd=[]
    │   ├─ Window slides:
    │   │   └─ Old: [████][ ][ ][ ]
    │   │   └─ New: [ ][████][ ][ ]
    │   └─ Now ready to send Frame 1
    │
    └─→ ✓ ACK received and window updated

    ┌─────────────────────────────────────────────────────────┐
    │ ✓✓✓ ACK RECEIVED AT SOURCE SUCCESSFULLY ✓✓✓          │
    │                                                          │
    │ Sender A: Window advanced                               │
    │ Can now send more frames (1,2,3,4...)                  │
    └─────────────────────────────────────────────────────────┘
```

### **REPLY MESSAGE (Application Response)**

```
═══════════════════════════════════════════════════════════════════════════════
TIME 16: SERVER B GENERATES REPLY MESSAGE
═══════════════════════════════════════════════════════════════════════════════

EndDevice B Application Layer:
    │
    └─→ sendApplicationData(destIP="10.0.0.1", data="Reply received...")
        │
        ├─→ Create ApplicationMessage:
        │   ├─ sourceIP: 10.0.1.2 (B)
        │   ├─ destinationIP: 10.0.0.1 (A)
        │   └─ payload: "Reply received at the network layer"
        │
        └─→ NetworkLayer::sendPacket()
            │
            ├─→ Create NetworkPacket:
            │   ├─ sourceIP: 10.0.1.2
            │   ├─ destinationIP: 10.0.0.1
            │   ├─ ttl: 30
            │   └─ payload: "Reply..."
            │
            ├─→ Lookup route for 10.0.0.1:
            │   ├─ Check routing tables
            │   ├─ Destination is in Subnet 1 (10.0.0.0/24)
            │   ├─ Next hop: Router R1
            │   └─ Next hop MAC: MAC_R1
            │
            └─→ Create Frame with NetworkPacket payload


═══════════════════════════════════════════════════════════════════════════════
TIME 17-18: FRAME TRANSMISSION FROM B TO ROUTER TO A (REVERSE PATH)
═══════════════════════════════════════════════════════════════════════════════

B → Hub H2 → Router R1 → Hub H1 → A
(Same as SOURCE→DESTINATION, but reversed IPs)


═══════════════════════════════════════════════════════════════════════════════
REPLY FLOW SUMMARY (B → A)
═══════════════════════════════════════════════════════════════════════════════

B's Application (generate reply)
    ↓
B's NetworkLayer (IP routing, dst=10.0.0.1, next hop=R1)
    ↓
B's DataLinkLayer (Frame, CSMA/CD)
    ↓
B's Physical (send on Hub H2)
    ↓
Hub H2 (broadcast)
    ↓
Router R1 (route lookup, TTL decrement)
    ↓
Router R1's Physical (send on Hub H1)
    ↓
Hub H1 (broadcast)
    ↓
A's Physical (receive frame)
    ↓
A's DataLinkLayer (extract, send ACK)
    ↓
A's NetworkLayer (extract IP packet, check dst=10.0.0.1? YES)
    ↓
A's Application (deliver message)
    ↓
✓ REPLY MESSAGE ARRIVES AT A
```

---

## **PROTOCOL STATE MACHINES**

### **CSMA/CD State Machine**

```
                    ┌─────────────┐
                    │  IDLE       │
                    └──────┬──────┘
                           │
                    Transmit requested
                           │
                           ▼
                    ┌─────────────────────┐
                    │ CARRIER SENSE       │◄──────┐
                    │ (Check channel)     │       │
                    └──────┬──────────────┘       │
                           │                      │
                ┌──────────┴──────────┐           │
                │                     │           │
            Idle?              Busy?  │           │
                │                     │           │
                │ YES              NO └──→ BACKOFF
                │                          (wait)
                │                          (retry)
                │                             │
                ▼                             └──┘
            ┌──────────────────┐
            │ TRANSMITTING     │
            └────────┬─────────┘
                     │
            Monitor during Tx
                     │
        ┌────────────┴───────────┐
        │                        │
   Collision?             No collision?
        │                        │
        │ YES                  NO│
        │                        │
        ▼                        ▼
    ┌─────────────┐        ┌──────────────┐
    │ JAM SIGNAL  │        │ SUCCESS      │
    └──────┬──────┘        └──────────────┘
           │
    Retry < 16?
           │
      YES  │  NO
      ┌────┘───┐
      │        │
      ▼        ▼
   BACKOFF  FRAME DROPPED
      │
      └──→ CARRIER SENSE (retry)
```

### **Go-Back-N Sender State Machine**

```
              ┌──────────────────┐
              │ READY_TO_SEND    │
              └────────┬─────────┘
                       │
          Frame available?
                       │
              ┌────────┴────────┐
              │                 │
            YES            NO   │
              │                 │ (wait)
              │                 │
              ▼                 ▼
         ┌────────────────┐  IDLE
         │ Send Frame    │
         │ (seqNum=next) │
         └────────┬──────┘
                  │
         ┌────────┴──────┐
         │               │
    Increment next    Timeout?
         │               │
         │ NO            │ YES
         ▼               ▼
    Check window      Retransmit
    full?             all (base to next-1)
         │               │
    ┌────┴────┐          │
    │         │          │
   YES       NO          │
    │         │          │
    │    READY_TO_SEND   │
    │         ▲          │
    │         │          │
    └─────────┘          │
                         │
    Receive ACK(k)       │
         │               │
         ▼               ▼
    Move base=k+1    Start timeout timer
         │               │
         └────────┬──────┘
                  │
              READY_TO_SEND
                  ▲
                  │
                  └─ (loop)
```

### **Go-Back-N Receiver State Machine**

```
         ┌──────────────────────┐
         │ WAITING              │
         │ (for seq=expected)   │
         └──────────┬───────────┘
                    │
        Receive Frame?
                    │
         ┌──────────┴──────────┐
         │                     │
         ▼                     ▼
    Frame seq       Frame seq ≠ expected
    == expected       (out-of-order)
         │                     │
         │                  Send ACK(expected-1)
         │                   [Duplicate ACK]
         │                     │
         │                  Discard frame
         │                     │
         ▼                     └──→ WAITING
    Accept Frame
         │
    Store in buffer
         │
    Send ACK(seq)
         │
    Increment expected
         │
         ▼
    WAITING (for seq=expected)
```

---

## **SERVICE ARCHITECTURE**

### **Echo Service (Port 7)**

```
Echo Request Message Flow:

Client:                   Echo Server:
(Port 5000)               (Port 7)
    │                         │
    │── TransportSegment ────→
    │   SrcPort: 5000         │
    │   DstPort: 7            │
    │   Payload: "Test"       │
    │                    (Handler invoked)
    │                         │
    │                    Process request
    │                         │
    │                    Print message
    │                         │
    │←── TransportSegment ───
    │   SrcPort: 7
    │   DstPort: 5000
    │   Payload: "Test" (echoed)
    │
(Echo received)
```

### **File Transfer Service (Port 21)**

```
FTP Session:

Client PUT Request:

Client:                   FTP Server:
(Port 5000)               (Port 21)
    │                         │
    │── "PUT|file.txt|..." ──→
    │   TransportSegment      │
    │   SrcPort: 5000         │
    │   DstPort: 21           │
    │                    (Handler parses)
    │                    filename: file.txt
    │                    content: ...
    │                    Store in session
    │                         │
    │←── "FILE_STORED" ───────
    │   TransportSegment
    │   (acknowledgment)
    │
(File transfer complete)


Active Sessions Map:
┌─────────────────────────────────────────────────┐
│ "10.0.0.1:5000:file.txt" →                     │
│  ├─ fileName: "file.txt"                       │
│  ├─ fileContent: "..."                         │
│  ├─ totalSegments: 1                           │
│  └─ receivedSegments: 1                        │
└─────────────────────────────────────────────────┘
```

---

## **TEST RESULTS & VALIDATION**

### **Submission 1: Data Link & Physical Layers**

```
Tests 1-7: ✅ PASS
├─ Test 1: CSMA/CD Normal Transmission ✓
├─ Test 2: CSMA/CD Carrier Sense (Busy Channel) ✓
├─ Test 3: CSMA/CD Collision Detection + Backoff ✓
├─ Test 4: Go-Back-N Send One Full Window ✓
├─ Test 5: Go-Back-N Window Full, Next Frame Waits ✓
├─ Test 6: Go-Back-N Timeout Retransmits ✓
└─ Test 7: Window Slides, Sender Can Send Next Frame ✓

Expected Output:
[CSMA/CD] Carrier sense, transmit success
[Channel] Collision detected, jam signal
[Go-Back-N] Window[████][ ][ ][ ]
[Go-Back-N] Cumulative ACK received, base slides
```

### **Submission 1.5: Bridge & Topology**

```
Test 10: ✅ PASS

Bridge MAC Table:
[Bridge BR1 MAC Table]
  MAC: MAC_C → Port 0

Topology Analysis:
[TOPOLOGY ANALYSIS REPORT]

--- COLLISION DOMAINS ---
Collision Domain 0: BR1
Collision Domain 1: C
Collision Domain 2: D

--- BROADCAST DOMAINS ---
Broadcast Domain 0: BR1 C D
```

### **Submission 2: Network Layer**

```
Test 11: ✅ CIDR IPv4 Parsing
[CIDR] 10.0.0.0/24 contains 10.0.0.5? YES ✓
[CIDR] 10.0.0.0/24 contains 10.0.0.15? YES ✓
[CIDR] 192.168.0.0/16 contains 192.168.1.100? YES ✓

Test 12: ✅ Longest-Prefix Route Matching
[Routing] 10.0.0.50 → 10.0.0.0/25 via 10.0.0.2 ✓
[Routing] 10.0.0.200 → 10.0.0.0/24 via 10.0.0.1 ✓
[Routing] 192.168.1.1 → 0.0.0.0/0 via 10.0.0.254 ✓

Test 13: ✅ ARP Protocol
[ARP Cache] Added: 10.0.0.1 → MAC_A ✓
[ARP] Lookup 10.0.0.1 → MAC_A ✓
[ARP] Lookup 10.0.0.100 → NOT FOUND ✓
```

### **Submission 3: Transport & Application**

```
Test 14: ✅ Echo Service
[Echo Service] Received echo request from 10.0.0.1:5000 ✓
[Echo Service] Request: Hello, Echo! ✓
[Echo Service] Echoing back to 10.0.0.1:5000 ✓

Test 15: ✅ File Transfer Service
[File Transfer Service] Received FTP request from 10.0.0.1:5000 ✓
[File Transfer Service] PUT document.txt (46 bytes) ✓
[File Transfer Service] File stored: document.txt ✓
```

---

## **FEATURE COMPLETENESS**

### **Submission 1: Physical & Data Link Layers**

| Feature | Implementation | Status |
|---------|-----------------|--------|
| Device hierarchy | Device→EndDevice, Hub, Switch, Bridge, Router | ✅ Complete |
| Star topology | Multiple hubs connecting devices | ✅ Complete |
| Frame structure | MAC addresses, payload, sequence, parity | ✅ Complete |
| CSMA/CD | Carrier sense, collision detect, exponential backoff (16 retries) | ✅ Complete |
| Collision handling | Jam signal, backoff, retry | ✅ Complete |
| MAC learning | Switch learns MAC→port mappings | ✅ Complete |
| Frame flooding | Unknown destination floods to all ports | ✅ Complete |
| Go-Back-N | Sliding window (N=4), cumulative ACKs, retransmit on timeout | ✅ Complete |
| Parity bits | Error detection on payloads | ✅ Complete |
| Bridge | MAC table, intelligent forwarding | ✅ Complete |
| Domain reporting | Collision & broadcast domain analysis | ✅ Complete |

### **Submission 2: Network Layer**

| Feature | Implementation | Status |
|---------|-----------------|--------|
| IPv4 addressing | Dotted decimal parsing (X.X.X.X) | ✅ Complete |
| CIDR notation | /8 to /32 subnet masks | ✅ Complete |
| Subnet matching | IP membership test with CIDR | ✅ Complete |
| Routing table | Add/remove/lookup routes | ✅ Complete |
| Longest-prefix | Selects most specific CIDR route | ✅ Complete |
| TTL handling | Decrement on each hop, drop at 0 | ✅ Complete |
| Multi-hop routing | Forward via routers between subnets | ✅ Complete |
| ARP protocol | IP↔MAC resolution & caching | ✅ Complete |
| Route display | Print routing table with metrics | ✅ Complete |
| Reverse path learning | Router learns return paths for ACKs | ✅ Complete |

### **Submission 3: Transport & Application**

| Feature | Implementation | Status |
|---------|-----------------|--------|
| Transport segment | Port fields (16-bit) + sequence/ACK | ✅ Complete |
| Port demultiplexing | Service handlers by destination port | ✅ Complete |
| Service registration | Register handlers on ports | ✅ Complete |
| Echo service | Port 7, request/reply | ✅ Complete |
| File Transfer service | Port 21, PUT/GET commands | ✅ Complete |
| Payload serialization | Serialize/deserialize data | ✅ Complete |
| Multi-layer encapsulation | App → Transport → Network → Link → Physical | ✅ Complete |
| Application delivery | Print messages at destination | ✅ Complete |

---

## **CONCLUSION**

This network simulator provides a comprehensive implementation of all OSI layers (Physical through Application) with realistic protocols:

✅ **Physical Layer**: Device topology with hubs, switches, bridges, and routers
✅ **Data Link**: CSMA/CD collision detection, Go-Back-N sliding window, MAC learning
✅ **Network**: CIDR routing with longest-prefix matching, ARP, multi-hop forwarding
✅ **Transport**: Port-based demultiplexing with service handlers
✅ **Application**: Echo and File Transfer services

All 15 tests validate end-to-end communication with proper layer encapsulation, error handling, and protocol state management.

