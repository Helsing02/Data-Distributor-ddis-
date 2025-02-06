#pragma once

#include <cstdint>

// Ethernet header structure
struct EthHdr {
    uint8_t destMac[6];
    uint8_t srcMac[6];
    uint16_t etherType;
};

// IP header structure
struct IpHdr {
    uint8_t  versionAndIhl;
    uint8_t  tos;
    uint16_t totalLength;
    uint16_t id;
    uint16_t flagsAndOffset;
    uint8_t  ttl;
    uint8_t  protocol;
    uint16_t checksum;
    uint32_t srcIp;
    uint32_t destIp;
};

// TCP header structure
struct TcpHdr {
    uint16_t srcPort;
    uint16_t destPort;
    uint32_t seqNum;
    uint32_t ackNum;
    uint16_t dataOffsetFlags;
    uint16_t window;
    uint16_t checksum;
    uint16_t urgentPtr;
};

// UDP header structure
struct UdpHdr {
    uint16_t srcPort;
    uint16_t destPort;
    uint16_t length;
    uint16_t checksum;
};

