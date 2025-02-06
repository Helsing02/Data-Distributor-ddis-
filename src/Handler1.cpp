#include "Handler.h"
#include "Utilities.h"

#include <iostream>

/**
 * This method checks the destination port of the packet. If the destination 
 * port is 7070, the packet is ignored, and a message is printed. Otherwise, 
 * the packet is written to the output file.
 */
void Handler1::handlePckt(PcapPacket& packet) {
    m_packetNum++;

    // If destination port is 7070, ignore the packet and print a message
    if (changeEndian(packet.tcpHdr.destPort) == 7070) {
        std::cout << "\033[32mОбработчик 1:\033[0m пакет под номером " << m_packetNum << " игнорируется\n";
    } else {
        // Write the packet's header and data to the output file
        m_outpFile.write((char*)&packet.pcapHdr, sizeof(packet.pcapHdr));
        m_outpFile.write(reinterpret_cast<const char*>(packet.data.data()), packet.pcapHdr.inclLen);
    }  
}
