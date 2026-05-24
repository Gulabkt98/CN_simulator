#ifndef APPLICATION_SERVICES_H
#define APPLICATION_SERVICES_H

#include <cstdint>
#include <string>
#include <map>
#include <iostream>
#include "../transport/TransportSegment.h"

using namespace std;

// ============ ECHO SERVICE ============
class EchoService {
public:
    static const uint16_t ECHO_PORT = 7;
    
    static void handleEchoRequest(const TransportSegment& segment, 
                           string srcIP, uint16_t srcPort);
};

// ============ FILE TRANSFER SERVICE ============
class FileTransferService {
public:
    static const uint16_t FTP_PORT = 21;
    
    struct FileTransferSession {
        string fileName;
        string fileContent;
        int totalSegments;
        int receivedSegments;
        
        FileTransferSession() 
            : totalSegments(0), receivedSegments(0) {}
    };
    
    static map<string, FileTransferSession> activeSessions;

    static void handleFTPRequest(const TransportSegment& segment, 
                                string srcIP, uint16_t srcPort);
};

#endif
