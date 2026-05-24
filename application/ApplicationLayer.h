#ifndef APPLICATIONLAYER_H
#define APPLICATIONLAYER_H

#include "ApplicationMessage.h"

class ApplicationLayer {

private:
    string hostName;

public:
    ApplicationLayer(string host = "");

    ApplicationMessage composeMessage(const string& sourceIP,
                                      const string& destinationIP,
                                      const string& data) const;

    void deliverMessage(const ApplicationMessage& message) const;
};

#endif
