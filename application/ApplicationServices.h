#ifndef APPLICATION_SERVICES_H
#define APPLICATION_SERVICES_H

#include <string>
#include <iostream>
#include "../transport/TransportSegment.h"

using namespace std;

// ============ ECHO SERVICE ============
class EchoService {
public:
    static const uint16_t ECHO_PORT = 7;
    
    static void handleEchoRequest(const TransportSegment& segment, 
                                 string srcIP, uint16_t srcPort) {
        cout << "\n[Echo Service] Received echo request from " 
             << srcIP << ":" << srcPort << endl;
        cout << "[Echo Service] Request: " << segment.payload << endl;
        cout << "[Echo Service] Echoing back to " << srcIP 
             << ":" << srcPort << endl;
    }
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
                                string srcIP, uint16_t srcPort) {
        cout << "\n[File Transfer Service] Received FTP request from " 
             << srcIP << ":" << srcPort << endl;
        cout << "[File Transfer Service] Request: " << segment.payload << endl;
        
        // Parse FTP command: "PUT|filename|content" or "GET|filename"
        string payload = segment.payload;
        size_t pos = 0;
        size_t next;
        
        next = payload.find("|", pos);
        string command = payload.substr(pos, next - pos);
        pos = next + 1;
        
        if (command == "PUT") {
            next = payload.find("|", pos);
            string fileName = payload.substr(pos, next - pos);
            pos = next + 1;
            string fileContent = payload.substr(pos);
            
            cout << "[File Transfer Service] PUT " << fileName 
                 << " (" << fileContent.length() << " bytes)" << endl;
            
            // Store file
            string key = srcIP + ":" + to_string(srcPort) + ":" + fileName;
            FileTransferSession session;
            session.fileName = fileName;
            session.fileContent = fileContent;
            session.totalSegments = 1;
            session.receivedSegments = 1;
            activeSessions[key] = session;
            
            cout << "[File Transfer Service] File stored: " << fileName << endl;
        } 
        else if (command == "GET") {
            string fileName = payload.substr(pos);
            cout << "[File Transfer Service] GET " << fileName << endl;
            cout << "[File Transfer Service] Retrieving file: " << fileName << endl;
        }
    }
};

// Static member initialization
map<string, FileTransferService::FileTransferSession> 
    FileTransferService::activeSessions;

#endif
