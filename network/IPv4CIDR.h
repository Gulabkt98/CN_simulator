#ifndef IPV4_CIDR_H
#define IPV4_CIDR_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

struct IPv4Address {
    uint32_t address;  // 32-bit IP address
    
    IPv4Address() : address(0) {}
    
    IPv4Address(string ipStr);
    
    IPv4Address(uint32_t addr) : address(addr) {}
    
    void parseIP(string ipStr);
    
    string toString() const;
    
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
    
    IPv4CIDR(string cidrStr);
    
    IPv4CIDR(IPv4Address net, int prefix) 
        : network(net), prefixLength(prefix) {}
    
    void parseCIDR(string cidrStr);
    
    string toString() const;
    
    uint32_t getMask() const;
    
    uint32_t getNetworkAddress() const;
    
    bool contains(const IPv4Address& ip) const;
    
    bool operator==(const IPv4CIDR& other) const {
        return network.address == other.network.address && 
               prefixLength == other.prefixLength;
    }
};

#endif
