#ifndef IPV4_CIDR_H
#define IPV4_CIDR_H

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

struct IPv4Address {
    uint32_t address;  // 32-bit IP address
    
    IPv4Address() : address(0) {}
    
    IPv4Address(string ipStr) {
        parseIP(ipStr);
    }
    
    IPv4Address(uint32_t addr) : address(addr) {}
    
    void parseIP(string ipStr) {
        int octets[4] = {0, 0, 0, 0};
        sscanf(ipStr.c_str(), "%d.%d.%d.%d", 
               &octets[0], &octets[1], &octets[2], &octets[3]);
        
        address = (octets[0] << 24) | (octets[1] << 16) | 
                  (octets[2] << 8) | octets[3];
    }
    
    string toString() const {
        int oct0 = (address >> 24) & 0xFF;
        int oct1 = (address >> 16) & 0xFF;
        int oct2 = (address >> 8) & 0xFF;
        int oct3 = address & 0xFF;
        
        char buf[16];
        snprintf(buf, sizeof(buf), "%d.%d.%d.%d", oct0, oct1, oct2, oct3);
        return string(buf);
    }
    
    bool operator==(const IPv4Address& other) const {
        return address == other.address;
    }
    
    bool operator!=(const IPv4Address& other) const {
        return address != other.address;
    }
};

struct IPv4CIDR {
    IPv4Address network;
    int prefixLength;  // /8 to /32
    
    IPv4CIDR() : prefixLength(32) {}
    
    IPv4CIDR(string cidrStr) {
        parseCIDR(cidrStr);
    }
    
    IPv4CIDR(IPv4Address net, int prefix) 
        : network(net), prefixLength(prefix) {}
    
    void parseCIDR(string cidrStr) {
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
    
    string toString() const {
        return network.toString() + "/" + to_string(prefixLength);
    }
    
    uint32_t getMask() const {
        if (prefixLength == 0) return 0;
        return (0xFFFFFFFFU << (32 - prefixLength));
    }
    
    uint32_t getNetworkAddress() const {
        return network.address & getMask();
    }
    
    bool contains(const IPv4Address& ip) const {
        return (ip.address & getMask()) == getNetworkAddress();
    }
    
    bool operator==(const IPv4CIDR& other) const {
        return network.address == other.network.address && 
               prefixLength == other.prefixLength;
    }
};

#endif
