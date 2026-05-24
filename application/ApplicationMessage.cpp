#include "ApplicationMessage.h"

ApplicationMessage::ApplicationMessage(string src, string dest, string data)
{
    sourceIP = src;
    destinationIP = dest;
    payload = data;
}