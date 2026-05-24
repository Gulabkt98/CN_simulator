
#include "devices/EndDevice.h"
#include "devices/Hub.h"
#include "devices/Router.h"
#include "devices/Bridge.h"
#include "network/Channel.h"
#include "network/AckBuffer.h"
#include "network/TopologyAnalysis.h"
#include "network/ARP.h"
#include "network/IPv4CIDR.h"
#include "network/RoutingTable.h"
#include "transport/TransportLayer.h"
#include "transport/TransportSegment.h"
#include "application/ApplicationServices.h"
#include <map>

void deliverACKs(const map<string, Device*>& deviceDirectory, Device* fallbackReceiver) {

    while(!AckBuffer::buffer.empty()) {

        Frame ack = AckBuffer::buffer.front();
        AckBuffer::buffer.pop();

        auto target = deviceDirectory.find(ack.destinationMAC);

        if (target != deviceDirectory.end()) {
            target->second->receive(ack, target->second);
        }
        else {
            fallbackReceiver->receive(ack, fallbackReceiver);
        }
    }
}

int main() {

    EndDevice A("A","MAC_A","10.0.0.1");
    EndDevice B("B","MAC_B","10.0.0.2");

    Hub H1("H1");

    map<string, Device*> deviceDirectory = {
        {"MAC_A", &A},
        {"MAC_B", &B}
    };

    A.connect(&H1);
    H1.connect(&A);

    B.connect(&H1);
    H1.connect(&B);

    Channel::reset();

    cout << "\n==CN_simulator =====\n\n";

    cout << "--- Test 1: CSMA/CD Normal Transmission ---\n\n";
    A.send("101101","MAC_B");

    cout << "\n--- Deliver ACK ---\n\n";
    deliverACKs(deviceDirectory, &A);

    cout << "\n--- Test 2: CSMA/CD Carrier Sense (Busy Channel) ---\n\n";
    Channel::busy = true;
    A.send("111000","MAC_B");
    Channel::reset();

    cout << "\n--- Test 3: CSMA/CD Collision Detection + Backoff ---\n\n";
    Channel::scheduleCollision();
    A.send("111000","MAC_B");

    cout << "\n--- Deliver ACK After Collision Recovery ---\n\n";
    deliverACKs(deviceDirectory, &A);

    cout << "\n--- Test 4: Go-Back-N Send One Full Window ---\n\n";
    A.send("110011","MAC_B");
    A.send("100111","MAC_B");
    A.send("110111","MAC_B");

    cout << "\n--- Test 5: Go-Back-N Window Full, Next Frame Waits ---\n\n";
    A.send("101010","MAC_B");

    cout << "\n--- Test 6: Go-Back-N Timeout Retransmits Outstanding Frames ---\n\n";
    A.timeout();

    cout << "\n--- Delivering Cumulative ACKs ---\n\n";
    deliverACKs(deviceDirectory, &A);

    cout << "\n--- Test 7: Window Slides, Sender Can Send Next Frame ---\n\n";
    A.send("101010","MAC_B");

    cout << "\n--- Delivering Final ACK ---\n\n";
    deliverACKs(deviceDirectory, &A);

    Hub H2("H2");
    Router R1("R1", "MAC_R1", "10.0.0.254");
    EndDevice A2("A2", "MAC_A2", "10.0.0.11");
    EndDevice B2("B2", "MAC_B2", "10.0.0.12");

    deviceDirectory["MAC_R1"] = &R1;
    deviceDirectory["MAC_A2"] = &A2;
    deviceDirectory["MAC_B2"] = &B2;

    A2.connect(&H1);
    H1.connect(&A2);

    H1.connect(&R1);
    R1.connect(&H1);

    R1.connect(&H2);
    H2.connect(&R1);

    B2.connect(&H2);
    H2.connect(&B2);

    A2.addRoute("10.0.0.12", "MAC_R1");
    B2.addRoute("10.0.0.11", "MAC_R1");

    R1.addRoute("10.0.0.12", &H2, "MAC_B2");
    R1.addRoute("10.0.0.11", &H1, "MAC_A2");

    cout << "\n--- Test 8: Multi-Hop Network Layer + Application Layer Simulation ---\n\n";
    A2.sendApplicationData("10.0.0.12", "Hello from the application layer, B2.");

    cout << "\n--- Deliver Application ACK ---\n\n";
    deliverACKs(deviceDirectory, &A2);

    cout << "\n--- Test 9: B2 sends an application reply back to A ---\n\n";
    B2.sendApplicationData("10.0.0.11", "Reply received at the network layer.");

    cout << "\n--- Deliver Reply ACK ---\n\n";
    deliverACKs(deviceDirectory, &B2);

    // Test 10: Bridge and Topology Analysis (Submission 1)
    cout << "\n========== SUBMISSION 1: BRIDGE & TOPOLOGY ANALYSIS ==========\n\n";
    
    Bridge BR1("BR1");
    EndDevice C("C", "MAC_C", "10.0.0.3");
    EndDevice D("D", "MAC_D", "10.0.0.4");
    
    C.connect(&BR1);
    BR1.connect(&C);
    
    D.connect(&BR1);
    BR1.connect(&D);
    
    cout << "\n--- Test 10: Bridge Forwarding and Domain Analysis ---\n\n";
    
    // Create a frame and send through bridge
    Frame testFrame("MAC_C", "MAC_D", "Test Frame from C to D", 0, false);
    BR1.receive(testFrame, &C);
    
    BR1.displayMACTable();
    
    // Analyze topology
    DomainReport report = TopologyAnalyzer::analyzeTopology(&BR1);
    TopologyAnalyzer::printDomainReport(report);

    // Test 11: IPv4 CIDR Parsing (Submission 2)
    cout << "\n========== SUBMISSION 2: CIDR IPv4 PARSING ==========\n\n";
    
    IPv4Address ip1("10.0.0.5");
    IPv4Address ip2("10.0.0.15");
    IPv4Address ip3("192.168.1.100");
    
    IPv4CIDR cidr1("10.0.0.0/24");
    IPv4CIDR cidr2("192.168.0.0/16");
    IPv4CIDR cidr3("172.16.0.0/12");
    
    cout << "\n--- Test 11: CIDR Network Testing ---\n";
    cout << "[CIDR] " << cidr1.toString() << " contains " << ip1.toString() 
         << "? " << (cidr1.contains(ip1) ? "YES" : "NO") << endl;
    cout << "[CIDR] " << cidr1.toString() << " contains " << ip2.toString() 
         << "? " << (cidr1.contains(ip2) ? "YES" : "NO") << endl;
    cout << "[CIDR] " << cidr2.toString() << " contains " << ip3.toString() 
         << "? " << (cidr2.contains(ip3) ? "YES" : "NO") << endl;
    
    // Test 12: Longest-Prefix Routing (Submission 2)
    cout << "\n--- Test 12: Longest-Prefix Route Matching ---\n";
    
    RoutingTable rtable;
    rtable.addRoute(IPv4CIDR("0.0.0.0/0"), "10.0.0.254", "MAC_GW", 1);        // Default route
    rtable.addRoute(IPv4CIDR("10.0.0.0/24"), "10.0.0.1", "MAC_A", 1);         // /24
    rtable.addRoute(IPv4CIDR("10.0.0.0/25"), "10.0.0.2", "MAC_B", 2);         // /25 (more specific)
    
    IPv4Address testIPs[] = {
        IPv4Address("10.0.0.50"),    // Should match /25 (most specific)
        IPv4Address("10.0.0.200"),   // Should match /24
        IPv4Address("192.168.1.1")   // Should match default route
    };
    
    for (const auto& testIP : testIPs) {
        RouteEntry* route = rtable.lookup(testIP);
        if (route) {
            cout << "[Routing] " << testIP.toString() << " -> " 
                 << route->destination.toString() 
                 << " via " << route->nextHopIP << endl;
        } else {
            cout << "[Routing] " << testIP.toString() << " -> NO ROUTE" << endl;
        }
    }
    rtable.displayTable();
    
    // Test 13: ARP Protocol (Submission 2)
    cout << "\n--- Test 13: ARP Cache and Resolution ---\n";
    ARPCache arpCache;
    arpCache.addEntry("10.0.0.1", "MAC_A");
    arpCache.addEntry("10.0.0.2", "MAC_B");
    arpCache.addEntry("10.0.0.254", "MAC_ROUTER");
    arpCache.displayCache();
    
    cout << "\n[ARP] Lookup 10.0.0.1 -> " << arpCache.lookup("10.0.0.1") << endl;
    cout << "[ARP] Lookup 10.0.0.100 (not in cache) -> " 
         << (arpCache.contains("10.0.0.100") ? "FOUND" : "NOT FOUND") << endl;
    
    // Test 14: Transport Layer Services (Submission 3)
    cout << "\n========== SUBMISSION 3: TRANSPORT LAYER & SERVICES ==========\n\n";
    
    TransportLayer transportA(5000);
    TransportLayer transportB(5001);
    
    // Register Echo service
    transportA.registerService(EchoService::ECHO_PORT, 
                              EchoService::handleEchoRequest);
    transportB.registerService(EchoService::ECHO_PORT, 
                              EchoService::handleEchoRequest);
    
    // Register FTP service
    transportA.registerService(FileTransferService::FTP_PORT, 
                              FileTransferService::handleFTPRequest);
    transportB.registerService(FileTransferService::FTP_PORT, 
                              FileTransferService::handleFTPRequest);
    
    cout << "\n--- Test 14: Echo Service ---\n";
    TransportSegment echoReq = transportA.createSegment(
        EchoService::ECHO_PORT, "Hello, Echo!", 1);
    transportB.receive(echoReq, "10.0.0.1");
    
    cout << "\n--- Test 15: File Transfer Service ---\n";
    TransportSegment ftpReq = transportA.createSegment(
        FileTransferService::FTP_PORT, 
        "PUT|document.txt|This is the file content that was transferred.",
        2);
    transportB.receive(ftpReq, "10.0.0.1");

    return 0;
}
