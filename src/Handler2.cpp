#include "Handler.h"

#include <iostream>
#include <algorithm>

/**
 * This method searches for the character 'x' in the Layer 4 header (either TCP or UDP). 
 * If 'x' is found, the packet is truncated at the position of 'x' and written to the output file.
 */
void Handler2::handlePckt(PcapPacket& packet) {
    size_t s;

    const uint8_t TCP_PROTOCOL = 0x06;
    if (packet.ipHdr.protocol == TCP_PROTOCOL) { // TCP
        s = sizeof(TcpHdr);                
    } else { // UDP
        s = sizeof(UdpHdr);
    }

    // Pointer to the Layer 4 header (either TCP or UDP)
    char* L4Header = (char*)&packet.tcpHdr;

    // Search for the character 'x' in the Layer 4 header
    char* found =  std::find(L4Header, L4Header + s, 'x');

    // If 'x' is found, truncate the packet at the position of 'x' and write it to the output file
    if (found != L4Header + s) {
        packet.pcapHdr.inclLen = found - L4Header + sizeof(EthHdr) + sizeof(IpHdr) + 1;
        m_outpFile.write((char*)&packet.pcapHdr, sizeof(packet.pcapHdr));
        m_outpFile.write(reinterpret_cast<const char*>(packet.data.data()), packet.pcapHdr.inclLen);
    }
}
