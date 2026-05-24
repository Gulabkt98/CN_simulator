#include "ApplicationLayer.h"
#include <iostream>

using namespace std;

ApplicationLayer::ApplicationLayer(string host)
{
    hostName = host;
}

ApplicationMessage ApplicationLayer::composeMessage(const string& sourceIP,
                                                    const string& destinationIP,
                                                    const string& data) const
{
    return ApplicationMessage(sourceIP, destinationIP, data);
}

void ApplicationLayer::deliverMessage(const ApplicationMessage& message) const
{
    cout << "[Application Layer] host [" << hostName << "] received message\n";
    cout << "  From IP: " << message.sourceIP << '\n';
    cout << "  To IP:   " << message.destinationIP << '\n';
    cout << "  Data:    " << message.payload << '\n';
}
