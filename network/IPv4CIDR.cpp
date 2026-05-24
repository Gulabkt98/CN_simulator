#include "IPv4CIDR.h"
#include <cstdio>

IPv4Address::IPv4Address(string ipStr)
{
    parseIP(ipStr);
}

void IPv4Address::parseIP(string ipStr)
{
    int octets[4] = {0, 0, 0, 0};
    sscanf(ipStr.c_str(), "%d.%d.%d.%d",
           &octets[0], &octets[1], &octets[2], &octets[3]);

    address = (octets[0] << 24) | (octets[1] << 16) |
              (octets[2] << 8) | octets[3];
}

string IPv4Address::toString() const
{
    int oct0 = (address >> 24) & 0xFF;
    int oct1 = (address >> 16) & 0xFF;
    int oct2 = (address >> 8) & 0xFF;
    int oct3 = address & 0xFF;

    char buf[16];
    snprintf(buf, sizeof(buf), "%d.%d.%d.%d", oct0, oct1, oct2, oct3);
    return string(buf);
}

IPv4CIDR::IPv4CIDR(string cidrStr)
{
    parseCIDR(cidrStr);
}

void IPv4CIDR::parseCIDR(string cidrStr)
{
    size_t slashPos = cidrStr.find('/');
    if (slashPos != string::npos) {
        string ipPart = cidrStr.substr(0, slashPos);
        string prefixPart = cidrStr.substr(slashPos + 1);

        network = IPv4Address(ipPart);
        prefixLength = stoi(prefixPart);
    } else {
        network = IPv4Address(cidrStr);
        prefixLength = 32;
    }
}

string IPv4CIDR::toString() const
{
    return network.toString() + "/" + to_string(prefixLength);
}

uint32_t IPv4CIDR::getMask() const
{
    if (prefixLength == 0) return 0;
    return (0xFFFFFFFFU << (32 - prefixLength));
}

uint32_t IPv4CIDR::getNetworkAddress() const
{
    return network.address & getMask();
}

bool IPv4CIDR::contains(const IPv4Address& ip) const
{
    return (ip.address & getMask()) == getNetworkAddress();
}