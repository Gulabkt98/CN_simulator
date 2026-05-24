#ifndef APPLICATIONMESSAGE_H
#define APPLICATIONMESSAGE_H

#include <string>

using namespace std;

class ApplicationMessage {

public:
    string sourceIP;
    string destinationIP;
    string payload;

    ApplicationMessage(string src = "", string dest = "", string data = "")
    {
        sourceIP = src;
        destinationIP = dest;
        payload = data;
    }
};

#endif
