#pragma once

#include <cstdint>
#include <vector>
#include "net_hdr_structs.h"

/**
 * @brief Global header for the PCAP file format.
 * 
 * @details Contains metadata about the PCAP file, such as 
 *          version, time zone correction, and network type.
 */
struct PcapGlobalHdr {
    uint32_t magicNumber;    ///< Magic number for identifying PCAP files.
    uint16_t versionMajor;   ///< Major version number of the PCAP file format.
    uint16_t versionMinor;   ///< Minor version number of the PCAP file format.
    int32_t  thisZone;       ///< Time zone correction (seconds from GMT).
    uint32_t sigFigs;        ///< Number of significant figures in timestamps.
    uint32_t snapLen;        ///< Maximum length of captured packets, in octets.
    uint32_t network;        ///< Data link type (e.g., Ethernet, PPP, etc.).
};

/**
 * @brief Packet header for each captured packet in a PCAP 
 *        file.
 * 
 * @details Contains timestamp information and the length of 
 *          the captured packet.
 */
struct PcapPacketHdr {
    uint32_t tsSec;          ///< Timestamp seconds (seconds since epoch).
    uint32_t tsUsec;         ///< Timestamp microseconds.
    uint32_t inclLen;        ///< Number of bytes captured from the packet.
    uint32_t origLen;        ///< Original length of the packet before truncation.
};

/**
 * @brief Structure representing a captured packet.
 * 
 * @details Contains the headers for the Ethernet, IP, and 
 *          either TCP or UDP layers, along with the data 
 *          section of the packet.
 */
struct PcapPacket {
    struct PcapPacketHdr pcapHdr;  ///< PCAP header for the packet.
    struct EthHdr ethHdr;          ///< Ethernet header.
    struct IpHdr ipHdr;            ///< IP header.
    
    /**
     * @brief Union representing either a TCP or UDP header.
     * 
     * @details Depending on the protocol, either the TCP or 
     *          UDP header will be used.
     */
    union {
        TcpHdr tcpHdr;       ///< TCP header.
        UdpHdr udpHdr;       ///< UDP header.
    };
    std::vector <uint8_t> data;    ///< Data payload of the packet.
};

