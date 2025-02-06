#pragma once

#include <cstdint>

/**
 * @brief Ethernet header structure.
 * 
 * @details Represents the Ethernet frame header, containing 
 *          destination MAC, source MAC, and the EtherType 
 *          field indicating the type of the upper-layer 
 *          protocol.
 */
struct EthHdr {
    uint8_t destMac[6];      ///< Destination MAC address.
    uint8_t srcMac[6];       ///< Source MAC address.
    uint16_t etherType;      ///< EtherType field to indicate the protocol type (e.g., IP, ARP).
};

/**
 * @brief IP header structure.
 * 
 * @details Represents the IPv4 header, containing fields for 
 *          version, header length, total length, 
 *          identification, flags, TTL, protocol, checksum, 
 *          and source/destination IP addresses.
 */
struct IpHdr {
    uint8_t  versionAndIhl;  ///< Version (4 bits) and header length (4 bits) of the IPv4 header.
    uint8_t  tos;            ///< Type of Service, used for quality of service (QoS).
    uint16_t totalLength;    ///< Total length of the IP packet (header + data).
    uint16_t id;             ///< Identification field for fragmentation.
    uint16_t flagsAndOffset; ///< Flags and fragment offset.
    uint8_t  ttl;            ///< Time-to-Live, used to limit the packet lifetime.
    uint8_t  protocol;       ///< Protocol (e.g., TCP = 6, UDP = 17).
    uint16_t checksum;       ///< Checksum for error-checking of the IP header.
    uint32_t srcIp;          ///< Source IP address (in network byte order).
    uint32_t destIp;         ///< Destination IP address (in network byte order).
};

/**
 * @brief TCP header structure.
 * 
 * @details Represents the Transmission Control Protocol (TCP) 
 *          header, containing fields for source and 
 *          destination ports, sequence/acknowledgment 
 *          numbers, flags, window size, checksum, and urgent 
 *          pointer.
 */
struct TcpHdr {
    uint16_t srcPort;        ///< Source port number.
    uint16_t destPort;       ///< Destination port number.
    uint32_t seqNum;         ///< Sequence number for the packet.
    uint32_t ackNum;         ///< Acknowledgment number for the packet.
    uint16_t dataOffsetFlags;///< Data offset (size of the TCP header) and flags (e.g., SYN, ACK).
    uint16_t window;         ///< Window size for flow control.
    uint16_t checksum;       ///< Checksum for error-checking the TCP header.
    uint16_t urgentPtr;      ///< Urgent pointer, used if the URG flag is set.
};

/**
 * @brief UDP header structure.
 * 
 * @ details Represents the User Datagram Protocol (UDP) 
 *           header, which contains source and destination 
 *           ports, length, and checksum for error-checking.
 */
struct UdpHdr {
    uint16_t srcPort;        ///< Source port number.
    uint16_t destPort;       ///< Destination port number.
    uint16_t length;         ///< Length of the UDP packet (header + data).
    uint16_t checksum;       ///< Checksum for error-checking the UDP header.
};

