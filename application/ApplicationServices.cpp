#include "ApplicationServices.h"

map<string, FileTransferService::FileTransferSession> FileTransferService::activeSessions;

void EchoService::handleEchoRequest(const TransportSegment& segment,
                                    string srcIP,
                                    uint16_t srcPort)
{
    cout << "\n[Echo Service] Received echo request from "
         << srcIP << ":" << srcPort << endl;
    cout << "[Echo Service] Request: " << segment.payload << endl;
    cout << "[Echo Service] Echoing back to " << srcIP
         << ":" << srcPort << endl;
}

void FileTransferService::handleFTPRequest(const TransportSegment& segment,
                                          string srcIP,
                                          uint16_t srcPort)
{
    cout << "\n[File Transfer Service] Received FTP request from "
         << srcIP << ":" << srcPort << endl;
    cout << "[File Transfer Service] Request: " << segment.payload << endl;

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